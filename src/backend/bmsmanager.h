#ifndef BMSMANAGER_H
#define BMSMANAGER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <memory>

#include "bmsserialreader.h"
#include "bmssnapshot.h"
#include "jbdprotocol.h"
#include "gpioFanController.h"

/// @brief BmsManager - Core BMS data manager and serial communication orchestrator
class BmsManager : public QObject
{
    Q_OBJECT

public:
    enum class ConnectionState {
        Idle = 0,           ///< Not connected
        Connecting,         ///< Attempting to connect
        Online,             ///< Connected and receiving data
        Degraded,           ///< Connected but with issues
        Error               ///< Connection error
    };
    Q_ENUM(ConnectionState)

    enum class ErrorCode {
        None = 0,           ///< No error
        InvalidConfig,      ///< Invalid configuration
        OpenFailed,         ///< Failed to open serial port
        PortClosed,         ///< Serial port closed unexpectedly
        WriteFailed,        ///< Failed to write to port
        Timeout,            ///< Communication timeout
        InvalidFrame,       ///< Invalid frame format
        ChecksumMismatch,   ///< Frame checksum error
        DeviceStatusError,  ///< BMS device status error
        TransportError,     ///< Serial transport error
        Unknown             ///< Unknown error
    };
    Q_ENUM(ErrorCode)

    Q_PROPERTY(bool connected READ isConnected NOTIFY connectedChanged)
    Q_PROPERTY(BmsManager::ConnectionState connectionState READ connectionState NOTIFY
                   connectionStateChanged)
    Q_PROPERTY(BmsManager::ErrorCode errorCode READ errorCode NOTIFY errorChanged)
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorChanged)
    Q_PROPERTY(QString statusMessage READ statusMessage NOTIFY statusMessageChanged)
    Q_PROPERTY(QString lastFrameHex READ lastFrameHex NOTIFY lastFrameHexChanged)

    explicit BmsManager(QObject *parent = nullptr);

    const BmsSnapshot &snapshot() const;
    bool isConnected() const;
    ConnectionState connectionState() const;
    ErrorCode errorCode() const;
    QString errorMessage() const;
    QString statusMessage() const;
    QString lastFrameHex() const;

    /// @brief Connect to BMS via serial port
    bool connectSerial(const QString &portName, int baudRate, int pollingIntervalMs = 1000);
    
    /// @brief Disconnect from BMS
    void disconnectSerial();
    
    /// @brief Request basic BMS information immediately
    void requestBasicInfo();
    
    /// @brief Get available serial ports
    QStringList availablePorts() const;

    /// @brief Initialize GPIO fan control
    Q_INVOKABLE bool initializeFanControl(int gpioPin, bool pwmMode = true, int pwmFrequency = 1000);
    
    /// @brief Set digital fan mode thresholds (ON/OFF)
    Q_INVOKABLE void setFanDigitalThresholds(double onTemp, double offTemp);
    
    /// @brief Set PWM mode temperature range
    Q_INVOKABLE void setFanPwmRange(double minTemp, double maxTemp, int minPwm = 20,
                                     int maxPwm = 100);
    
    /// @brief Set fan state (Digital mode)
    Q_INVOKABLE void setFanState(bool on);
    
    /// @brief Set fan speed (PWM mode)
    Q_INVOKABLE void setFanSpeed(int pwmPercent);
    
    /// @brief Check if fan control is initialized
    Q_INVOKABLE bool isFanInitialized() const;

signals:
    void snapshotChanged();
    void connectedChanged();
    void connectionStateChanged();
    void errorChanged();
    void statusMessageChanged();
    void lastFrameHexChanged();

private slots:
    void handleFrame(const QByteArray &frame);
    void handleReaderConnectionChanged(bool connected);
    void handleReaderTransportError(BmsSerialReader::TransportErrorCode code,
                                    const QString &errorMessage,
                                    bool recoverable);
    void handlePollTimeout();
    void handleResponseTimeout();
    void handleReconnectTimeout();

private:
    bool attemptConnect();
    bool sendBasicInfoRequest();
    bool sendCellVoltagesRequest();
    bool sendTelemetryRequests();
    void scheduleReconnect(const QString &reason);
    void clearReconnect();

    void setConnectionState(ConnectionState state);
    void setError(ErrorCode code, const QString &message);
    void clearError();
    void setStatusMessage(const QString &message);

    BmsSerialReader m_reader;
    BmsSnapshot m_snapshot;
    std::unique_ptr<GpioFanController> m_fanController;

    bool m_connected = false;
    bool m_shouldStayConnected = false;
    bool m_waitingForResponse = false;

    QString m_portName;
    int m_baudRate = 9600;
    int m_pollingIntervalMs = 1000;

    ConnectionState m_connectionState = ConnectionState::Idle;
    ErrorCode m_errorCode = ErrorCode::None;
    QString m_errorMessage;
    QString m_statusMessage;
    QString m_lastFrameHex;

    QTimer m_pollTimer;
    QTimer m_responseTimeoutTimer;
    QTimer m_reconnectTimer;
    int m_reconnectAttempt = 0;
};

#endif // BMSMANAGER_H
