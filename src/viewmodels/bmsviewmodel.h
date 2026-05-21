#ifndef BMSVIEWMODEL_H
#define BMSVIEWMODEL_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVariantMap>

#include "../backend/bmsmanager.h"

/// @brief BmsViewModel - Qt MVVM bridge for BMS data
class BmsViewModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVariantMap snapshot READ snapshot NOTIFY snapshotChanged)
    Q_PROPERTY(bool connected READ isConnected NOTIFY connectedChanged)
    Q_PROPERTY(int connectionState READ connectionState NOTIFY connectionStateChanged)
    Q_PROPERTY(QString connectionStateText READ connectionStateText NOTIFY connectionStateChanged)
    Q_PROPERTY(int errorCode READ errorCode NOTIFY errorChanged)
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorChanged)
    Q_PROPERTY(QString statusMessage READ statusMessage NOTIFY statusMessageChanged)
    Q_PROPERTY(QString lastFrameHex READ lastFrameHex NOTIFY lastFrameHexChanged)
    Q_PROPERTY(QStringList availablePorts READ availablePorts NOTIFY availablePortsChanged)
    Q_PROPERTY(QString portName READ portName WRITE setPortName NOTIFY portNameChanged)
    Q_PROPERTY(int baudRate READ baudRate WRITE setBaudRate NOTIFY baudRateChanged)
    Q_PROPERTY(int pollingIntervalMs READ pollingIntervalMs WRITE setPollingIntervalMs NOTIFY pollingIntervalMsChanged)

public:
    explicit BmsViewModel(QObject *parent = nullptr);

    QVariantMap snapshot() const;
    bool isConnected() const;
    int connectionState() const;
    QString connectionStateText() const;
    int errorCode() const;
    QString errorMessage() const;
    QString statusMessage() const;
    QString lastFrameHex() const;
    QStringList availablePorts() const;
    QString portName() const;
    int baudRate() const;
    int pollingIntervalMs() const;
    void setPortName(const QString &portName);
    void setBaudRate(int baudRate);
    void setPollingIntervalMs(int intervalMs);

    /// @brief Connect to BMS via UART
    Q_INVOKABLE void connectToUart();
    
    /// @brief Disconnect from BMS
    Q_INVOKABLE void disconnectFromUart();
    
    /// @brief Request BMS data immediately
    Q_INVOKABLE void refreshNow();
    
    /// @brief Reload available ports list
    Q_INVOKABLE void reloadAvailablePorts();

signals:
    void snapshotChanged();
    void connectedChanged();
    void connectionStateChanged();
    void errorChanged();
    void statusMessageChanged();
    void lastFrameHexChanged();
    void availablePortsChanged();
    void portNameChanged();
    void baudRateChanged();
    void pollingIntervalMsChanged();

private:
    BmsManager m_manager;

    QStringList m_availablePorts;
    QString m_portName;
    int m_baudRate = 9600;
    int m_pollingIntervalMs = 1000;
};

#endif // BMSVIEWMODEL_H
