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
    _networks = QList<WifiNetwork*>();
}

WifiDevice::~WifiDevice()
{
    if (!_device)
        delete _device;
}

QString WifiDevice::name()
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
    emit networkAdded();
}

void WifiDevice::_onNetworkRemoved(QVariant network)
{
    emit networkRemoved();
}

const QList<WifiNetwork*>& WifiDevice::listNetworks()
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
        _networks.append(new WifiNetwork(qdbus_cast<QString>(arg)));
    }
    arg.endArray();

    _device->bindToSignal(this, SLOT(_onNtworkAdded(QVariant)), "AccessPointAdded", "org.freedesktop.NetworkManager.Device.Wireless");
    _device->bindToSignal(this, SLOT(_onNetworkRemoved(QVariant)), "AccessPointRemoved", "org.freedesktop.NetworkManager.Device.Wireless");
    return _networks;
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
