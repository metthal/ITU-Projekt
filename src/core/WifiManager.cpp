#include "WifiManager.h"
#include <QDBusArgument>

WifiManager::WifiManager()
{
    _networkManager = new NetworkManagerDBusObject("/org/freedesktop/NetworkManager");
    connect(_networkManager, SIGNAL(responseReceived(QDBusMessage)), this, SLOT(_listDevicesFinished(QDBusMessage)));
}

WifiManager::~WifiManager()
{
    if (_networkManager)
        delete _networkManager;
}

void WifiManager::listDevices()
{
    _networkManager->call("GetDevices");
}

void WifiManager::_listDevicesFinished(QDBusMessage response)
{
    if (response.type() != QDBusMessage::ReplyMessage)
    {
        emit listDevicesError("Error in communication with NetworkManager.");
        return;
    }

    if (!response.arguments()[0].canConvert<QDBusArgument>())
    {
        emit listDevicesError("Error in communication with NetworkManager.");
        return;
    }

    const QDBusArgument& arg = response.arguments()[0].value<QDBusArgument>();
    if (arg.currentType() != QDBusArgument::ArrayType)
    {
        emit listDevicesError("Error in communication with NetworkManager.");
        return;
    }

    arg.beginArray();
    while (!arg.atEnd())
    {
        WifiDevice* device = new WifiDevice(qdbus_cast<QString>(arg));
        if (!device->isValid())
            continue;

        _devices.append(device);
    }
    arg.endArray();

    emit listDevicesFinished(_devices);
}
