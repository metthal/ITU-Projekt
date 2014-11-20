#include <QDBusArgument>
#include "WifiManager.h"
#include "databasesqlite.h"
#include "Exception.h"

WifiManager::WifiManager(DatabaseSQLite* db)
{
    _db = db;
    _networkManager = new NetworkManagerDBusObject("/org/freedesktop/NetworkManager");
}

WifiManager::~WifiManager()
{
    if (_networkManager)
        delete _networkManager;
}

void WifiManager::loadDatabase(const QString& path)
{
    _db->open(path);
    _networks = _db->getNetworks();
    _db->close();
}

void WifiManager::storeDatabase(const QString& path)
{
    _db->open(path);
    for (WifiNetwork* network : _networks)
    {
        network->lastSeen(QDateTime::currentDateTime());
        db.log(network);
    }
    _db->close();
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
