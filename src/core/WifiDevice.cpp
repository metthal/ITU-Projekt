#include <QDBusArgument>
#include "WifiDevice.h"
#include "Exception.h"

WifiDevice::WifiDevice(const QString& path)
{
    _device = new NetworkManagerDBusObject(path);
    _name = "";
    _validated = false;
    _valid = false;
    _requestName();
}

WifiDevice::~WifiDevice()
{
    if (!_device)
        delete _device;
}

const QString& WifiDevice::name() const
{
    return _name;
}

bool WifiDevice::isValid()
{
    if (_validated)
        return _valid;

    QVariantList args;
    args << QString("org.freedesktop.NetworkManager.Device") << QString("DeviceType");
    QDBusMessage response = _device->call("Get", args);

    // TODO: check response
    const QDBusVariant& arg = response.arguments()[0].value<QDBusVariant>();
    uint32_t deviceType = qvariant_cast<uint32_t>(arg.variant());
    _validated = true;
    _valid = deviceType == NM_DEVICE_TYPE_WIFI;
    return _valid;
}

void WifiDevice::_onNetworkAdded(QVariant network)
{
    // TODO add to list

    emit networkAdded();
}

void WifiDevice::_onNetworkRemoved(QVariant network)
{
    // TODO remove from list

    emit networkRemoved();
}

void WifiDevice::loadNetworks(QList<WifiNetwork*>& networks)
{
    QDBusMessage response = _device->call("GetAccessPoints");
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
        QString path = qdbus_cast<QString>(arg);
        WifiNetwork* newNetwork = new WifiNetwork(path);
        bool found = false;
        for (WifiNetwork* network : networks)
        {
            if (*newNetwork == *network)
            {
                delete newNetwork;
                newNetwork = network;
                found = true;
                break;
            }
        }

        if (found)
            newNetwork->assignToObject(path);
        else
            networks.append(newNetwork);
    }
    arg.endArray();

    _device->bindToSignal(this, SLOT(_onNtworkAdded(QVariant)), "AccessPointAdded", "org.freedesktop.NetworkManager.Device.Wireless");
    _device->bindToSignal(this, SLOT(_onNetworkRemoved(QVariant)), "AccessPointRemoved", "org.freedesktop.NetworkManager.Device.Wireless");
}

void WifiDevice::_requestName()
{
    QVariantList args;
    args << QString("org.freedesktop.NetworkManager.Device") << QString("Interface");
    QDBusMessage response = _device->call("Get", args);
    if (response.type() != QDBusMessage::ReplyMessage)
        return;

    if (!response.arguments()[0].canConvert<QDBusVariant>())
        return;

    const QDBusVariant& arg = response.arguments()[0].value<QDBusVariant>();
    if (!arg.variant().canConvert<QString>())
        return;

    _name = qvariant_cast<QString>(arg.variant());
}
