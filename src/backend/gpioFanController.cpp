#include "gpioFanController.h"

#include <QStandardPaths>
#include <QCoreApplication>
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <cmath>
#include <QtGlobal>

GpioFanController::GpioFanController(QObject *parent)
    : QObject(parent), m_pythonProcess(this)
{
    connect(&m_pythonProcess,
            qOverload<int, QProcess::ExitStatus>(&QProcess::finished),
            this,
            &GpioFanController::handlePythonProcessFinished);
    
    connect(&m_pythonProcess,
            static_cast<void (QProcess::*)(QProcess::ProcessError)>(&QProcess::errorOccurred),
            this,
            &GpioFanController::handlePythonProcessError);
    
    connect(&m_pythonProcess, &QProcess::readyReadStandardOutput, this,
            &GpioFanController::handlePythonStdout);
    connect(&m_pythonProcess, &QProcess::readyReadStandardError, this,
            &GpioFanController::handlePythonStderr);

    m_debounceTimer.setSingleShot(true);
    m_debounceTimer.setInterval(100);  // Debounce 100ms
    connect(&m_debounceTimer, &QTimer::timeout, [this]() {
        if (m_pendingUpdate) {
            m_pendingUpdate = false;
            if (m_mode == FanMode::Digital) {
                // updateTemperature()
            }
        }
    });
}

GpioFanController::~GpioFanController()
{
    cleanup();
}

bool GpioFanController::initialize(int gpioPin, FanMode mode, int pwmFrequency)
{
    if (m_initialized) {
        cleanup();
    }

    m_gpioPin = gpioPin;
    m_mode = mode;
    m_pwmFrequency = qMax(1, pwmFrequency);

    // find Python script in common locations
    QString appDir = QCoreApplication::applicationDirPath();
    QString candidates[] = {
        QDir(appDir).filePath("gpio_fan_controller.py"),
        QDir(appDir).filePath("../src/backend/gpio_fan_controller.py"),
        QDir(appDir).filePath("../../src/backend/gpio_fan_controller.py"),
        "/usr/local/bin/gpio_fan_controller.py",
        "/opt/bms/gpio_fan_controller.py",
    };

    for (const auto &path : candidates) {
        if (QFileInfo(path).exists()) {
            m_pythonScriptPath = path;
            break;
        }
    }

    if (m_pythonScriptPath.isEmpty()) {
        emit errorOccurred(
            "gpio_fan_controller.py not found. Please ensure the Python script is installed.");
        return false;
    }

    // Test initialize GPIO using Python script
    QStringList args;
    args << m_pythonScriptPath << "init" << QString::number(gpioPin)
         << (mode == FanMode::PWM ? "pwm" : "digital") << QString::number(pwmFrequency);

    m_pythonProcess.start("python3", args);
    if (!m_pythonProcess.waitForFinished(5000)) {
        emit errorOccurred("Failed to initialize GPIO (timeout)");
        return false;
    }

    if (m_pythonProcess.exitCode() != 0) {
        QString initError = QString::fromUtf8(m_pythonProcess.readAllStandardError());
        emit errorOccurred(QString("GPIO initialization failed: %1").arg(initError));
        return false;
    }

    m_initialized = true;
    emit initialized();
    return true;
}

void GpioFanController::cleanup()
{
    if (!m_initialized) {
        return;
    }

    setFanState(false);

    m_debounceTimer.stop();
    if (m_pythonProcess.state() == QProcess::Running) {
        m_pythonProcess.terminate();
        if (!m_pythonProcess.waitForFinished(2000)) {
            m_pythonProcess.kill();
        }
    }

    m_initialized = false;
    m_fanOn = false;
    m_currentPwm = 0;
}

void GpioFanController::updateTemperature(double temperatureC)
{
    if (!m_initialized) {
        return;
    }

    m_currentTemperature = temperatureC;
    emit temperatureUpdated(temperatureC);

    if (m_mode == FanMode::Digital) {
        // Hysteresis logic
        bool shouldBeOn = m_fanOn ? (temperatureC > m_digitalOffTemp)
                                  : (temperatureC > m_digitalOnTemp);

        if (shouldBeOn != m_fanOn) {
            setFanState(shouldBeOn);
        }
    } else {
        // PWM mode
        int newPwm = calculatePwmFromTemperature(temperatureC);
        if (newPwm != m_currentPwm) {
            setFanSpeed(newPwm);
        }
    }
}

void GpioFanController::setDigitalThresholds(double onTemp, double offTemp)
{
    if (onTemp > offTemp) {
        // Swap if needed
        qSwap(onTemp, offTemp);
    }
    m_digitalOnTemp = onTemp;
    m_digitalOffTemp = offTemp;
}

void GpioFanController::setPwmRange(double minTemp, double maxTemp, int minPwm, int maxPwm)
{
    if (minTemp > maxTemp) {
        qSwap(minTemp, maxTemp);
    }
    if (minPwm > maxPwm) {
        qSwap(minPwm, maxPwm);
    }

    m_pwmMinTemp = minTemp;
    m_pwmMaxTemp = maxTemp;
    m_pwmMin = qBound(0, minPwm, 100);
    m_pwmMax = qBound(0, maxPwm, 100);
}

void GpioFanController::setFanState(bool on)
{
    if (!m_initialized || m_mode != FanMode::Digital) {
        return;
    }

    if (on == m_fanOn) {
        return;
    }

    m_fanOn = on;
    m_currentPwm = on ? 100 : 0;

    QStringList args;
    args << m_pythonScriptPath << "set_digital" << QString::number(m_gpioPin) << (on ? "1" : "0");

    executePythonCommand(args);

    emit fanStateChanged(on);
}

void GpioFanController::setFanSpeed(int pwmPercent)
{
    if (!m_initialized || m_mode != FanMode::PWM) {
        return;
    }

    pwmPercent = qBound(0, pwmPercent, 100);

    if (pwmPercent == m_currentPwm) {
        return;
    }

    m_currentPwm = pwmPercent;

    QStringList args;
    args << m_pythonScriptPath << "set_pwm" << QString::number(m_gpioPin)
         << QString::number(pwmPercent);

    executePythonCommand(args);

    emit fanSpeedChanged(pwmPercent);
}

int GpioFanController::calculatePwmFromTemperature(double temp)
{
    // Linear interpolation
    // minTemp -> minPwm%, maxTemp -> maxPwm%

    if (temp <= m_pwmMinTemp) {
        return m_pwmMin;
    }
    if (temp >= m_pwmMaxTemp) {
        return m_pwmMax;
    }

    // Linear interpolation
    double ratio = (temp - m_pwmMinTemp) / (m_pwmMaxTemp - m_pwmMinTemp);
    int pwm = static_cast<int>(m_pwmMin + ratio * (m_pwmMax - m_pwmMin) + 0.5);

    return qBound(m_pwmMin, pwm, m_pwmMax);
}

void GpioFanController::executePythonCommand(const QStringList &args)
{
    // Running async, no wait
    if (m_pythonProcess.state() != QProcess::NotRunning) {
        m_pythonProcess.waitForFinished(100);
    }

    m_pythonProcess.start("python3", args, QProcess::Unbuffered | QProcess::ReadOnly);
}

void GpioFanController::handlePythonProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitStatus != QProcess::NormalExit || exitCode != 0) {
        QString errOutput = QString::fromUtf8(m_pythonProcess.readAllStandardError());
        if (!errOutput.isEmpty()) {
            emit errorOccurred(QString("Python script error: %1").arg(errOutput));
        }
    }
}

void GpioFanController::handlePythonProcessError(QProcess::ProcessError error)
{
    QString errorStr;
    switch (error) {
    case QProcess::FailedToStart:
        errorStr = "Failed to start python3 process";
        break;
    case QProcess::Crashed:
        errorStr = "Python process crashed";
        break;
    case QProcess::Timedout:
        errorStr = "Python process timed out";
        break;
    case QProcess::WriteError:
        errorStr = "Failed to write to python process";
        break;
    case QProcess::ReadError:
        errorStr = "Failed to read from python process";
        break;
    default:
        errorStr = "Unknown python process error";
        break;
    }
    emit errorOccurred(errorStr);
}

void GpioFanController::handlePythonStdout()
{
    QString output = QString::fromUtf8(m_pythonProcess.readAllStandardOutput()).trimmed();
    if (!output.isEmpty()) {
        qDebug().noquote() << "[GPIO]" << output;
    }
}

void GpioFanController::handlePythonStderr()
{
    QString errMsg = QString::fromUtf8(m_pythonProcess.readAllStandardError()).trimmed();
    if (!errMsg.isEmpty()) {
        qWarning().noquote() << "[GPIO Error]" << errMsg;
    }
}
