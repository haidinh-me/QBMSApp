#include "bmsviewmodel.h"

#include <algorithm>
#include <QVariantList>

BmsViewModel::BmsViewModel(QObject *parent)
    : QObject(parent)
{
    connect(&m_manager, &BmsManager::snapshotChanged, this, &BmsViewModel::snapshotChanged);
    connect(&m_manager, &BmsManager::connectedChanged, this, &BmsViewModel::connectedChanged);
    connect(&m_manager,
            &BmsManager::connectionStateChanged,
            this,
            &BmsViewModel::connectionStateChanged);
    connect(&m_manager, &BmsManager::errorChanged, this, &BmsViewModel::errorChanged);
    connect(&m_manager, &BmsManager::statusMessageChanged, this, &BmsViewModel::statusMessageChanged);
    connect(&m_manager, &BmsManager::lastFrameHexChanged, this, &BmsViewModel::lastFrameHexChanged);

    reloadAvailablePorts();
    if (!m_availablePorts.isEmpty()) {
        m_portName = m_availablePorts.first();
    }

#ifdef Q_OS_LINUX
    if (m_portName.isEmpty()) {
        m_portName = QStringLiteral("/dev/ttyAMA0");
    }
#else
    if (m_portName.isEmpty()) {
        m_portName = QStringLiteral("COM3");
    }
#endif
}

QVariantMap BmsViewModel::snapshot() const
{
    const BmsSnapshot &snapshot = m_manager.snapshot();

    QVariantList cells;
    cells.reserve(snapshot.cells.size());
    for (double cell : snapshot.cells) {
        cells.push_back(cell);
    }

    QVariantList alarms;
    alarms.reserve(snapshot.alarms.size());
    for (const QString &alarm : snapshot.alarms) {
        alarms.push_back(alarm);
    }

    return {
        {QStringLiteral("timestamp"), snapshot.timestamp.toString(Qt::ISODate)},
        {QStringLiteral("packVoltage"), snapshot.packVoltage},
        {QStringLiteral("current"), snapshot.current},
        {QStringLiteral("soc"), snapshot.soc},
        {QStringLiteral("temperatureC"), snapshot.temperatureC},
        {QStringLiteral("remainingCapacityAh"), snapshot.remainingCapacityAh},
        {QStringLiteral("designCapacityAh"), snapshot.designCapacityAh},
        {QStringLiteral("cycleLife"), snapshot.cycleLife},
        {QStringLiteral("alarms"), alarms},
        {QStringLiteral("cells"), cells}};
}

bool BmsViewModel::isConnected() const {return m_manager.isConnected();}

int BmsViewModel::connectionState() const
{
    return static_cast<int>(m_manager.connectionState());
}

QString BmsViewModel::connectionStateText() const
{
    const auto state = m_manager.connectionState();
    if (state == BmsManager::ConnectionState::Idle) {
        return QStringLiteral("Idle");
    }
    if (state == BmsManager::ConnectionState::Connecting) {
        return QStringLiteral("Connecting");
    }
    if (state == BmsManager::ConnectionState::Online) {
        return QStringLiteral("Online");
    }
    if (state == BmsManager::ConnectionState::Degraded) {
        return QStringLiteral("Degraded");
    }
    return QStringLiteral("Error");
}

int BmsViewModel::errorCode() const
{
    return static_cast<int>(m_manager.errorCode());
}

QString BmsViewModel::errorMessage() const
{
    return m_manager.errorMessage();
}

QString BmsViewModel::statusMessage() const {return m_manager.statusMessage();}

QString BmsViewModel::lastFrameHex() const {return m_manager.lastFrameHex();}

QStringList BmsViewModel::availablePorts() const {return m_availablePorts;}

QString BmsViewModel::portName() const {return m_portName;}

int BmsViewModel::baudRate() const {return m_baudRate;}

int BmsViewModel::pollingIntervalMs() const {return m_pollingIntervalMs;}

void BmsViewModel::setPortName(const QString &portName)
{
    const QString trimmedPort = portName.trimmed();
    if (m_portName == trimmedPort) {
        return;
    }

    m_portName = trimmedPort;
    emit portNameChanged();
}

void BmsViewModel::setBaudRate(int baudRate)
{
    const int safeBaudRate = std::max(1200, baudRate);
    if (m_baudRate == safeBaudRate) {
        return;
    }

    m_baudRate = safeBaudRate;
    emit baudRateChanged();
}

void BmsViewModel::setPollingIntervalMs(int intervalMs)
{
    const int safeIntervalMs = std::max(200, intervalMs);
    if (m_pollingIntervalMs == safeIntervalMs) {
        return;
    }

    m_pollingIntervalMs = safeIntervalMs;
    emit pollingIntervalMsChanged();
}

void BmsViewModel::connectToUart()
{
    m_manager.connectSerial(m_portName, m_baudRate, m_pollingIntervalMs);
}

void BmsViewModel::disconnectFromUart() {m_manager.disconnectSerial();}

void BmsViewModel::refreshNow() {m_manager.requestBasicInfo();}

void BmsViewModel::reloadAvailablePorts()
{
    const QStringList ports = m_manager.availablePorts();
    if (m_availablePorts == ports) {
        return;
    }

    m_availablePorts = ports;
    emit availablePortsChanged();
}
