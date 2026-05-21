#include "bmsserialreader.h"

#include <QIODevice>
#include <QSerialPortInfo>

BmsSerialReader::BmsSerialReader(QObject *parent)
    : QObject(parent)
{
    connect(&m_serial, &QSerialPort::readyRead, this, &BmsSerialReader::onReadyRead);
    connect(&m_serial,
            &QSerialPort::errorOccurred,
            this,
            [this](QSerialPort::SerialPortError error) {
                if (error == QSerialPort::NoError) {
                    return;
                }

                TransportErrorCode code = TransportErrorCode::ReadFailed;
                bool recoverable = false;
                if (error == QSerialPort::ResourceError) {
                    code = TransportErrorCode::ResourceLost;
                    recoverable = true;
                }

                emit transportErrorOccurred(code, m_serial.errorString(), recoverable);
                if (error == QSerialPort::ResourceError) {
                    disconnectPort();
                }
            });
}

bool BmsSerialReader::connectPort(const QString &portName, int baudRate)
{
    if (m_serial.isOpen()) {
        disconnectPort();
    }

    m_serial.setPortName(portName);
    m_serial.setBaudRate(baudRate);
    m_serial.setDataBits(QSerialPort::Data8);
    m_serial.setParity(QSerialPort::NoParity);
    m_serial.setStopBits(QSerialPort::OneStop);
    m_serial.setFlowControl(QSerialPort::NoFlowControl);

    if (!m_serial.open(QIODevice::ReadWrite)) {
        emit transportErrorOccurred(TransportErrorCode::PortOpenFailed,
                                    QStringLiteral("Cannot open %1: %2")
                                        .arg(portName, m_serial.errorString()),
                                    false);
        return false;
    }

    m_rxBuffer.clear();
    emit connectionChanged(true);
    return true;
}

void BmsSerialReader::disconnectPort()
{
    const bool wasConnected = m_serial.isOpen();

    m_rxBuffer.clear();

    if (wasConnected) {
        m_serial.close();
        emit connectionChanged(false);
    }
}

bool BmsSerialReader::isConnected() const
{
    return m_serial.isOpen();
}

bool BmsSerialReader::writeFrame(const QByteArray &frame)
{
    if (!m_serial.isOpen()) {
        emit transportErrorOccurred(TransportErrorCode::PortClosed,
                                    QStringLiteral("Cannot write: serial port is closed."),
                                    true);
        return false;
    }

    const qint64 writtenBytes = m_serial.write(frame);
    if (writtenBytes != frame.size()) {
        emit transportErrorOccurred(TransportErrorCode::WriteFailed,
                                    QStringLiteral("Failed to write full request frame."),
                                    true);
        return false;
    }

    return true;
}

QStringList BmsSerialReader::availablePorts()
{
    QStringList ports;
    const auto serialPorts = QSerialPortInfo::availablePorts();
    ports.reserve(serialPorts.size());

    for (const QSerialPortInfo &port : serialPorts) {
        QString displayName = port.systemLocation();
        if (displayName.isEmpty()) {
            displayName = port.portName();
        }
        if (displayName.startsWith(QStringLiteral("\\\\.\\"))) {
            displayName = port.portName();
        }
        ports.push_back(displayName);
    }

    return ports;
}

void BmsSerialReader::onReadyRead()
{
    m_rxBuffer.append(m_serial.readAll());
    processIncomingBuffer();
}

void BmsSerialReader::processIncomingBuffer()
{
    while (true) {
        const int frameStart = m_rxBuffer.indexOf(char(0xDD));
        if (frameStart < 0) {
            m_rxBuffer.clear();
            return;
        }

        if (frameStart > 0) {
            m_rxBuffer.remove(0, frameStart);
        }

        if (m_rxBuffer.size() < 4) {
            return;
        }

        const quint8 dataLength = static_cast<quint8>(m_rxBuffer[3]);
        const int expectedFrameLength = 7 + static_cast<int>(dataLength);
        if (m_rxBuffer.size() < expectedFrameLength) {
            return;
        }

        const quint8 frameEnd = static_cast<quint8>(m_rxBuffer[expectedFrameLength - 1]);
        if (frameEnd != 0x77) {
            m_rxBuffer.remove(0, 1);
            continue;
        }

        const QByteArray frame = m_rxBuffer.left(expectedFrameLength);
        m_rxBuffer.remove(0, expectedFrameLength);
        emit frameReceived(frame);
    }
}
