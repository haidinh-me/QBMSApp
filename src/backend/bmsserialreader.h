#ifndef BMSSERIALREADER_H
#define BMSSERIALREADER_H

#include <QByteArray>
#include <QObject>
#include <QSerialPort>
#include <QStringList>

/// @brief BmsSerialReader - Serial port handler for UART communication with BMS
class BmsSerialReader : public QObject
{
    Q_OBJECT

public:
    enum class TransportErrorCode {
        PortOpenFailed = 1,     ///< Failed to open serial port
        PortClosed = 2,         ///< Port closed unexpectedly
        WriteFailed = 3,        ///< Failed to write data
        ResourceLost = 4,       ///< Resource lost (e.g., device unplugged)
        ReadFailed = 5          ///< Failed to read data
    };
    Q_ENUM(TransportErrorCode)

public:
    explicit BmsSerialReader(QObject *parent = nullptr);

    /// @brief Connect to serial port
    bool connectPort(const QString &portName, int baudRate);
    
    /// @brief Disconnect from serial port
    void disconnectPort();
    
    /// @brief Check if port is connected
    bool isConnected() const;
    
    /// @brief Write frame to serial port
    bool writeFrame(const QByteArray &frame);

    /// @brief Get list of available serial ports
    static QStringList availablePorts();

signals:
    void frameReceived(const QByteArray &frame);
    void connectionChanged(bool connected);
    void transportErrorOccurred(BmsSerialReader::TransportErrorCode code,
                                const QString &message,
                                bool recoverable);

private slots:
    void onReadyRead();

private:
    /// @brief Process incoming data buffer
    void processIncomingBuffer();

    QSerialPort m_serial;       ///< Serial port instance
    QByteArray m_rxBuffer;      ///< Receive buffer
};

#endif // BMSSERIALREADER_H
