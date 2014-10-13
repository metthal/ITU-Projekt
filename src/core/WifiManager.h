#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <QObject>
#include <QDBusMessage>
#include <QStringList>
#include "NetworkManagerDBusObject.h"
#include "WifiDevice.h"

class WifiManager : public QObject
{
    Q_OBJECT

public:
    WifiManager();
    virtual ~WifiManager();

    void listDevices();

signals:
    void listDevicesFinished(const QList<WifiDevice*>& deviceList);
    void listDevicesError(QString error);

private:
    NetworkManagerDBusObject* _networkManager;
    QList<WifiDevice*> _devices;
};

#endif // WIFIMANAGER_H
