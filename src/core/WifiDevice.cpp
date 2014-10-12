#include "WifiDevice.h"

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

QString WifiDevice::name()
{
    return _name;
}

bool WifiDevice::isValid()
{
    if (_validated)
        return _valid;

    QList<QVariant> args;
    args << QString("org.freedesktop.NetworkManager.Device") << QString("DeviceType");
    QDBusMessage response = _device->directCall("Get", args);

    // TODO: check response
    const QDBusVariant& arg = response.arguments()[0].value<QDBusVariant>();
    uint32_t deviceType = qvariant_cast<uint32_t>(arg.variant());
    _validated = true;
    _valid = deviceType == NM_DEVICE_TYPE_WIFI;
    return _valid;
}

void WifiDevice::_requestName()
{
    connect(_device, SIGNAL(responseReceived(QDBusMessage)), this, SLOT(_requestNameFinished(QDBusMessage)));

    QList<QVariant> args;
    args << QString("org.freedesktop.NetworkManager.Device") << QString("Interface");
    _device->call("Get", args);
}

void WifiDevice::_requestNameFinished(QDBusMessage response)
{
    if (response.type() != QDBusMessage::ReplyMessage)
        return;

    if (!response.arguments()[0].canConvert<QDBusVariant>())
        return;

    const QDBusVariant& arg = response.arguments()[0].value<QDBusVariant>();
    if (!arg.variant().canConvert<QString>())
        return;

    _name = qvariant_cast<QString>(arg.variant());
}
