#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <QObject>
#include "NetworkManagerDBusObject.h"
#include "WifiDevice.h"
#include "databasesqlite.h"

class WifiManager : public QObject
{
    Q_OBJECT

public:
    WifiManager(DatabaseSQLite* db);
    virtual ~WifiManager();

    void loadDatabase(const QString& path);
    void storeDatabase(const QString& path);

    void loadDevices();
    const QList<WifiDevice*>& devices() const;

    void loadNetworks(WifiDevice* device);
    const QList<WifiNetwork*>& networks() const;

private:
    DatabaseSQLite* _db;
    NetworkManagerDBusObject* _networkManager;
    QList<WifiDevice*> _devices;
    QList<WifiNetwork*> _networks;
};

#endif // WIFIMANAGER_H
