#include "bmsmanager.h"

#include <QTime>
#include <QtGlobal>

namespace {
constexpr int kMinPollingIntervalMs = 200;
constexpr int kMinReconnectDelayMs = 1000;
constexpr int kMaxReconnectDelayMs = 15000;
}

BmsManager::BmsManager(QObject *parent)
    : QObject(parent)
{
    m_snapshot.packVoltage = 25.6;
    m_snapshot.current = 12.4;
    m_snapshot.soc = 50;
    m_snapshot.temperatureC = 12.2;
    m_snapshot.remainingCapacityAh = 10.0;
    m_snapshot.designCapacityAh = 12.0;
    m_snapshot.cycleLife = 22;
    m_snapshot.cells = QVector<double>(8, 3.3545f);

    m_pollTimer.setInterval(m_pollingIntervalMs);
    m_responseTimeoutTimer.setSingleShot(true);
    m_reconnectTimer.setSingleShot(true);

    connect(&m_reader, &BmsSerialReader::frameReceived, this, &BmsManager::handleFrame);
    connect(&m_reader,
            &BmsSerialReader::connectionChanged,
            this,
            &BmsManager::handleReaderConnectionChanged);
    connect(&m_reader,
            &BmsSerialReader::transportErrorOccurred,
            this,
            &BmsManager::handleReaderTransportError);
    connect(&m_pollTimer, &QTimer::timeout, this, &BmsManager::handlePollTimeout);
    connect(&m_responseTimeoutTimer, &QTimer::timeout, this, &BmsManager::handleResponseTimeout);
    connect(&m_reconnectTimer, &QTimer::timeout, this, &BmsManager::handleReconnectTimeout);

    this->initializeFanControl(17, true, 1000);   
    this->setFanPwmRange(30.0, 50.0, 20, 100);

    setStatusMessage(QStringLiteral("Idle. Configure UART and connect."));
}

const BmsSnapshot &BmsManager::snapshot() const{ return m_snapshot; }

bool BmsManager::isConnected() const{ return m_connected; }

BmsManager::ConnectionState BmsManager::connectionState() const{ return m_connectionState; }

BmsManager::ErrorCode BmsManager::errorCode() const{ return m_errorCode; }

QString BmsManager::errorMessage() const { return m_errorMessage; }

QString BmsManager::statusMessage() const { return m_statusMessage; }

QString BmsManager::lastFrameHex() const { return m_lastFrameHex; }

bool BmsManager::connectSerial(const QString &portName, int baudRate, int pollingIntervalMs)
{
    const QString trimmedPortName = portName.trimmed();
    if (trimmedPortName.isEmpty()) {
        setConnectionState(ConnectionState::Error);
        setError(ErrorCode::InvalidConfig, QStringLiteral("UART port name is empty."));
        setStatusMessage(QStringLiteral("UART port name is empty."));
        return false;
    }

    m_portName = trimmedPortName;
    m_baudRate = qMax(1200, baudRate);
    m_pollingIntervalMs = qMax(kMinPollingIntervalMs, pollingIntervalMs);
    m_pollTimer.setInterval(m_pollingIntervalMs);

    m_shouldStayConnected = true;
    setConnectionState(ConnectionState::Connecting);
    setStatusMessage(QStringLiteral("Connecting to %1...").arg(m_portName));

    return attemptConnect();
}

bool BmsManager::attemptConnect()
{
    if (!m_shouldStayConnected) {
        return false;
    }

    if (!m_reader.connectPort(m_portName, m_baudRate)) {
        setConnectionState(ConnectionState::Error);
        setError(ErrorCode::OpenFailed,
                 QStringLiteral("Cannot open %1 at %2 baud.").arg(m_portName).arg(m_baudRate));
        scheduleReconnect(QStringLiteral("Open failed, retrying..."));
        return false;
    }

    clearReconnect();
    clearError();
    setConnectionState(ConnectionState::Online);
    m_pollTimer.start();
    return sendTelemetryRequests();
}

bool BmsManager::sendBasicInfoRequest()
{
    if (!m_reader.writeFrame(JbdProtocol::buildBasicInfoRequest())) {
        setConnectionState(ConnectionState::Degraded);
        setError(ErrorCode::WriteFailed, QStringLiteral("Failed to request BMS basic info."));
        return false;
    }

    m_waitingForResponse = true;
    const int timeoutMs = qMax(1000, m_pollingIntervalMs * 2);
    m_responseTimeoutTimer.start(timeoutMs);
    return true;
}

bool BmsManager::sendCellVoltagesRequest()
{
    if (!m_reader.writeFrame(JbdProtocol::buildCellVoltagesRequest())) {
        setConnectionState(ConnectionState::Degraded);
        setError(ErrorCode::WriteFailed, QStringLiteral("Failed to request cell voltages."));
        return false;
    }

    return true;
}

bool BmsManager::sendTelemetryRequests()
{
    if (!sendBasicInfoRequest()) {
        return false;
    }

    if (!sendCellVoltagesRequest()) {
        return false;
    }

    return true;
}

void BmsManager::disconnectSerial()
{
    m_shouldStayConnected = false;
    m_waitingForResponse = false;
    clearReconnect();

    m_pollTimer.stop();
    m_responseTimeoutTimer.stop();
    m_reader.disconnectPort();

    setConnectionState(ConnectionState::Idle);
    clearError();
    setStatusMessage(QStringLiteral("Serial port disconnected."));
}

void BmsManager::requestBasicInfo()
{
    if (!m_reader.isConnected()) {
        setConnectionState(ConnectionState::Degraded);
        setError(ErrorCode::PortClosed, QStringLiteral("Serial link is closed."));
        if (m_shouldStayConnected) {
            scheduleReconnect(QStringLiteral("Link closed, reconnecting..."));
        }
        return;
    }

    if (!sendTelemetryRequests()) {
        if (m_shouldStayConnected) {
            scheduleReconnect(QStringLiteral("Write failed, reconnecting..."));
        }
        return;
    }

    if (m_connectionState != ConnectionState::Online) {
        setConnectionState(ConnectionState::Online);
    }

    setStatusMessage(QStringLiteral("Polling BMS..."));
}

QStringList BmsManager::availablePorts() const { return BmsSerialReader::availablePorts(); }

void BmsManager::handlePollTimeout() { requestBasicInfo(); }

void BmsManager::handleResponseTimeout()
{
    if (!m_waitingForResponse) {
        return;
    }

    m_waitingForResponse = false;
    setConnectionState(ConnectionState::Degraded);
    setError(ErrorCode::Timeout, QStringLiteral("No response from BMS in time."));
    setStatusMessage(QStringLiteral("Response timeout, waiting for next poll."));

    if (m_shouldStayConnected) {
        scheduleReconnect(QStringLiteral("No response, retrying UART link..."));
    }
}

void BmsManager::handleReconnectTimeout()
{
    if (!m_shouldStayConnected) {
        return;
    }

    setConnectionState(ConnectionState::Connecting);
    setStatusMessage(QStringLiteral("Reconnect attempt #%1...").arg(m_reconnectAttempt + 1));
    attemptConnect();
}

void BmsManager::scheduleReconnect(const QString &reason)
{
    if (!m_shouldStayConnected) {
        return;
    }

    ++m_reconnectAttempt;
    const int delayMs = qMin(kMinReconnectDelayMs << qMin(m_reconnectAttempt - 1, 4),
                             kMaxReconnectDelayMs);

    setStatusMessage(QStringLiteral("%1 (retry in %2 ms)").arg(reason).arg(delayMs));
    if (!m_reconnectTimer.isActive()) {
        m_reconnectTimer.start(delayMs);
    }
}

void BmsManager::clearReconnect()
{
    m_reconnectTimer.stop();
    m_reconnectAttempt = 0;
}

void BmsManager::handleFrame(const QByteArray &frame)
{
    m_waitingForResponse = false;
    m_responseTimeoutTimer.stop();

    const QString frameHex = QString::fromLatin1(frame.toHex(' ')).toUpper();
    if (m_lastFrameHex != frameHex) {
        m_lastFrameHex = frameHex;
        emit lastFrameHexChanged();
    }

    const JbdProtocol::ParseResult parsed = JbdProtocol::parseFrame(frame);
    if (!parsed.ok) {
        ErrorCode code = ErrorCode::InvalidFrame;
        if (parsed.errorCode == JbdProtocol::ParseErrorCode::ChecksumMismatch) {
            code = ErrorCode::ChecksumMismatch;
        } else if (parsed.errorCode == JbdProtocol::ParseErrorCode::BmsStatusError) {
            code = ErrorCode::DeviceStatusError;
        }

        setConnectionState(ConnectionState::Degraded);
        setError(code, parsed.errorMessage);
        setStatusMessage(QStringLiteral("Invalid frame: %1").arg(parsed.errorMessage));
        return;
    }

    if (parsed.frameType == JbdProtocol::FrameType::BasicInfo) {
        m_snapshot.packVoltage = parsed.snapshot.packVoltage;
        m_snapshot.current = parsed.snapshot.current;
        m_snapshot.soc = parsed.snapshot.soc;
        m_snapshot.temperatureC = parsed.snapshot.temperatureC;
        m_snapshot.remainingCapacityAh = parsed.snapshot.remainingCapacityAh;
        m_snapshot.designCapacityAh = parsed.snapshot.designCapacityAh;
        m_snapshot.cycleLife = parsed.snapshot.cycleLife;
        m_snapshot.alarms = parsed.snapshot.alarms;

        if (m_fanController && m_fanController->isInitialized()) {
            m_fanController->updateTemperature(m_snapshot.temperatureC);
        }
    } else if (parsed.frameType == JbdProtocol::FrameType::CellVoltages) {
        m_snapshot.cells = parsed.snapshot.cells;
    }

    m_snapshot.timestamp = QDateTime::currentDateTime();
    emit snapshotChanged();

    clearError();
    setConnectionState(ConnectionState::Online);

    setStatusMessage(
        QStringLiteral("Data updated at %1").arg(QTime::currentTime().toString("HH:mm:ss")));
}

void BmsManager::handleReaderConnectionChanged(bool connected)
{
    if (m_connected == connected) {
        return;
    }

    m_connected = connected;
    emit connectedChanged();

    if (connected) {
        clearError();
        clearReconnect();
        setConnectionState(ConnectionState::Online);
        setStatusMessage(QStringLiteral("UART link is online."));
        return;
    }

    m_waitingForResponse = false;
    m_responseTimeoutTimer.stop();

    if (!m_shouldStayConnected) {
        setConnectionState(ConnectionState::Idle);
        return;
    }

    setConnectionState(ConnectionState::Degraded);
    setError(ErrorCode::PortClosed, QStringLiteral("UART link is offline."));
    scheduleReconnect(QStringLiteral("UART link lost."));
}

void BmsManager::handleReaderTransportError(BmsSerialReader::TransportErrorCode code,
                                            const QString &errorMessage,
                                            bool recoverable)
{
    ErrorCode mapped = ErrorCode::TransportError;
    if (code == BmsSerialReader::TransportErrorCode::PortOpenFailed) {
        mapped = ErrorCode::OpenFailed;
    } else if (code == BmsSerialReader::TransportErrorCode::PortClosed) {
        mapped = ErrorCode::PortClosed;
    } else if (code == BmsSerialReader::TransportErrorCode::WriteFailed) {
        mapped = ErrorCode::WriteFailed;
    }

    setConnectionState(ConnectionState::Error);
    setError(mapped, errorMessage);
    setStatusMessage(QStringLiteral("Serial error: %1").arg(errorMessage));

    if (recoverable && m_shouldStayConnected) {
        scheduleReconnect(QStringLiteral("Temporary transport error."));
    }
}

void BmsManager::setConnectionState(ConnectionState state)
{
    if (m_connectionState == state) {
        return;
    }

    m_connectionState = state;
    emit connectionStateChanged();
}

void BmsManager::setError(ErrorCode code, const QString &message)
{
    if (m_errorCode == code && m_errorMessage == message) {
        return;
    }

    m_errorCode = code;
    m_errorMessage = message;
    emit errorChanged();
}

void BmsManager::clearError() { setError(ErrorCode::None, QString()); }

void BmsManager::setStatusMessage(const QString &message)
{
    if (m_statusMessage == message) {
        return;
    }

    m_statusMessage = message;
    emit statusMessageChanged();
}

// ====================================================
// ============ Fan Control Implementation ============
// ====================================================

bool BmsManager::initializeFanControl(int gpioPin, bool pwmMode, int pwmFrequency)
{
    if (!m_fanController) {
        m_fanController = std::make_unique<GpioFanController>(this);
    }

    GpioFanController::FanMode mode = pwmMode ? GpioFanController::FanMode::PWM
                                              : GpioFanController::FanMode::Digital;

    if (!m_fanController->initialize(gpioPin, mode, pwmFrequency)) {
        setStatusMessage(
            QStringLiteral("Fan control initialization failed for GPIO pin %1").arg(gpioPin));
        return false;
    }

    setStatusMessage(QStringLiteral("Fan control initialized on GPIO %1 (%2 mode)")
                         .arg(gpioPin)
                         .arg(pwmMode ? "PWM" : "Digital"));
    return true;
}

void BmsManager::setFanDigitalThresholds(double onTemp, double offTemp)
{
    if (m_fanController) {
        m_fanController->setDigitalThresholds(onTemp, offTemp);
    }
}

void BmsManager::setFanPwmRange(double minTemp, double maxTemp, int minPwm, int maxPwm)
{
    if (m_fanController) {
        m_fanController->setPwmRange(minTemp, maxTemp, minPwm, maxPwm);
    }
}

void BmsManager::setFanState(bool on)
{
    if (m_fanController) {
        m_fanController->setFanState(on);
    }
}

void BmsManager::setFanSpeed(int pwmPercent)
{
    if (m_fanController) {
        m_fanController->setFanSpeed(pwmPercent);
    }
}

bool BmsManager::isFanInitialized() const
{
    return m_fanController && m_fanController->isInitialized();
}
