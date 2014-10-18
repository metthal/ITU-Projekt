#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <QObject>
#include "NetworkManagerDBusObject.h"
#include "WifiDevice.h"

class WifiManager : public QObject
{
    Q_OBJECT

public:
    WifiManager();
    virtual ~WifiManager();

    void listDevices();
    const QList<WifiDevice*>& devices() const;

private:
    NetworkManagerDBusObject* _networkManager;
    QList<WifiDevice*> _devices;
};

#endif // WIFIMANAGER_H
