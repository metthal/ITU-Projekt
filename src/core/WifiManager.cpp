#include <QDBusArgument>
#include "WifiManager.h"
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

void WifiManager::listDevices()
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
            continue;

        _devices.append(device);
    }
    arg.endArray();
}

const QList<WifiDevice*>& WifiManager::devices() const
{
    return _devices;
}
