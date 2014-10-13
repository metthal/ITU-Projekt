#ifndef WIFI_DEVICE_H
#define WIFI_DEVICE_H

#include <QObject>
#include "NetworkManagerDBusObject.h"
#include "WifiNetwork.h"

enum DeviceType
{
    NM_DEVICE_TYPE_WIFI = 2
};

class WifiDevice : public QObject
{
    Q_OBJECT

public:
    WifiDevice(const QString& path);
    virtual ~WifiDevice();

    bool isValid();
    QString name();

    void listNetworks();

signals:
    void listNetworksFinished();
    void listNetworksError(QString error);

public slots:
    void networkAdded(QVariant network);
    void networkRemoved(QVariant network);

private slots:
    void _requestNameFinished(QDBusMessage response);
    void _listNetworksFinished(QDBusMessage response);

private:
    void _requestName();

    NetworkManagerDBusObject* _device;
    QString _name;
    bool _validated;
    bool _valid;
    QList<WifiNetwork*> _networks;
};

#endif // WIFI_DEVICE_H
