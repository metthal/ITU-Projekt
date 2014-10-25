#include <QDBusArgument>
#include "WifiManager.h"
#include "databasesqlite.h"
#include "Exception.h"

WifiManager::WifiManager()
{
    _networkManager = new NetworkManagerDBusObject("/org/freedesktop/NetworkManager");
}

WifiManager::~WifiManager()
{
    if (_networkManager)
        delete _networkManager;
}

void WifiManager::loadDatabase(const QString& path)
{
    DatabaseSQLite db(path);
    db.open();
    _networks = db.getNetworks();
    db.close();
}

void WifiManager::storeDatabase(const QString& path)
{
    DatabaseSQLite db(path);
    db.open();
    for (WifiNetwork* network : _networks)
        db.log(network);
    db.close();
}

void WifiManager::loadDevices()
{
    QDBusMessage response = _networkManager->call("GetDevices");
    if (response.type() != QDBusMessage::ReplyMessage)
        throw Exception("Error in communication with NetworkManager.");

    if (!response.arguments()[0].canConvert<QDBusArgument>())
        throw Exception("Error in communication with NetworkManager.");

    const QDBusArgument& arg = response.arguments()[0].value<QDBusArgument>();
    if (arg.currentType() != QDBusArgument::ArrayType)
        throw Exception("Error in communication with NetworkManager.");

    arg.beginArray();
    while (!arg.atEnd())
    {
        WifiDevice* device = new WifiDevice(qdbus_cast<QString>(arg));
        if (!device->isValid())
        {
            delete device;
            continue;
        }

        _devices.append(device);
    }
    arg.endArray();
}

const QList<WifiDevice*>& WifiManager::devices() const
{
    return _devices;
}

void WifiManager::loadNetworks(WifiDevice* device)
{
    device->loadNetworks(_networks);
}

const QList<WifiNetwork*>& WifiManager::networks() const
{
    return _networks;
}
