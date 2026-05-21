#ifndef GPIOFANCONTROLLER_H
#define GPIOFANCONTROLLER_H

#include <QObject>
#include <QString>
#include <QProcess>
#include <QTimer>

/// @brief GpioFanController - GPIO fan control for Raspberry Pi
class GpioFanController : public QObject
{
    Q_OBJECT

public:
    enum class FanMode {
        Digital,  ///< Digital ON/OFF mode
        PWM       ///< PWM speed control (0-100%)
    };
    Q_ENUM(FanMode)

    explicit GpioFanController(QObject *parent = nullptr);
    ~GpioFanController();

    /// @brief Initialize GPIO fan control
    bool initialize(int gpioPin, FanMode mode, int pwmFrequency = 25000);

    /// @brief Stop GPIO fan control and cleanup
    void cleanup();

    /// @brief Update fan speed based on temperature
    void updateTemperature(double temperatureC);

    /// @brief Set digital mode thresholds
    void setDigitalThresholds(double onTemp, double offTemp);

    /// @brief Set PWM mode temperature-to-speed mapping
    void setPwmRange(double minTemp, double maxTemp, int minPwm = 20, int maxPwm = 100);

    /// @brief Set fan state directly (Digital mode)
    void setFanState(bool on);

    /// @brief Set fan speed directly (PWM mode)
    void setFanSpeed(int pwmPercent);

    bool isFanOn() const { return m_fanOn; }
    int currentPwm() const { return m_currentPwm; }
    double currentTemperature() const { return m_currentTemperature; }
    bool isInitialized() const { return m_initialized; }
    FanMode mode() const { return m_mode; }

signals:
    void fanStateChanged(bool on);
    void fanSpeedChanged(int pwmPercent);
    void temperatureUpdated(double temp);
    void errorOccurred(const QString &message);
    void initialized();

private slots:
    /// @brief Handle Python subprocess finished signal
    void handlePythonProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    
    /// @brief Handle Python subprocess error
    void handlePythonProcessError(QProcess::ProcessError error);
    
    /// @brief Handle Python subprocess stdout output
    void handlePythonStdout();
    
    /// @brief Handle Python subprocess stderr output
    void handlePythonStderr();

private:
    /// @brief Execute Python GPIO control script
    /// @param args Command line arguments to pass
    ///
    /// Spawns pigpio-based Python script as subprocess.
    /// This is safe to call multiple times; each call creates a new process
    /// that connects to the persistent pigpiod daemon.
    void executePythonCommand(const QStringList &args);

    /// @brief Calculate PWM percentage from temperature
    int calculatePwmFromTemperature(double temp);

    bool m_initialized = false;             ///< Initialization flag
    FanMode m_mode = FanMode::PWM;          ///< Current fan mode
    int m_gpioPin = -1;                     ///< GPIO pin number
    int m_pwmFrequency = 25000;             ///< PWM frequency (Hz)

    // Digital mode parameters
    double m_digitalOnTemp = 40.0;          ///< Temperature to turn ON (°C)
    double m_digitalOffTemp = 35.0;         ///< Temperature to turn OFF (°C) - hysteresis
    bool m_fanOn = false;                   ///< Current fan state

    // PWM mode parameters
    double m_pwmMinTemp = 30.0;             ///< Min temp for PWM (°C)
    double m_pwmMaxTemp = 50.0;             ///< Max temp for PWM (°C)
    int m_pwmMin = 20;                      ///< Min PWM percentage (%)
    int m_pwmMax = 100;                     ///< Max PWM percentage (%)
    int m_currentPwm = 0;                   ///< Current PWM value (%)

    double m_currentTemperature = 0.0;      ///< Last read temperature (°C)
    QProcess m_pythonProcess;               ///< Python subprocess
    QString m_pythonScriptPath;             ///< Path to Python control script

    // Debounce mechanism to prevent duplicate commands
    QTimer m_debounceTimer;                 ///< Debounce timer
    bool m_pendingUpdate = false;           ///< Pending update flag
};

#endif // GPIOFANCONTROLLER_H
