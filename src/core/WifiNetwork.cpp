#include "WifiNetwork.h"
#include <QDBusArgument>

WifiNetwork::WifiNetwork(const QString& path)
{
    _id = -1;
    _network = nullptr;
    _quality = 0;
    assignToObject(path);
    _firstSeen = QDateTime::currentDateTime();
}

WifiNetwork::WifiNetwork(int32_t id, const QString& ssid, const QString& bssid, const QDateTime& firstSeen, const QDateTime& lastSeen)
{
    _network = nullptr;
    _id = id;
    _ssid = QString(ssid);
    _bssid = QString(bssid);
    _quality = 0;
    _frequency = 0;
    _flags = NM_802_11_AP_FLAGS_NONE;
    _secFlags = NM_802_11_AP_SEC_NONE;
    _mode = NM_802_11_MODE_UNKNOWN;
    _firstSeen = firstSeen;
    _lastSeen = lastSeen;
}

WifiNetwork::~WifiNetwork()
{
    if (_network != nullptr)
        delete _network;
}

bool WifiNetwork::operator==(const WifiNetwork& rhs)
{
    return ((_ssid == rhs._ssid) && (_bssid == rhs._bssid));
}

bool WifiNetwork::operator!=(const WifiNetwork& rhs)
{
    return !(*this == rhs);
}

bool WifiNetwork::operator>(const WifiNetwork& rhs)
{
    if (_quality > rhs._quality)
        return true;

    if ((_quality == rhs._quality) && (_ssid > rhs._ssid))
        return true;

    return false;
}

bool WifiNetwork::operator<(const WifiNetwork& rhs)
{
    if (_quality < rhs._quality)
        return true;

    if ((_quality == rhs._quality) && (_ssid < rhs._ssid))
        return true;

    return false;
}

void WifiNetwork::assignToObject(const QString& path)
{
    if (_network != nullptr)
        delete _network;

    _network = new NetworkManagerDBusObject(path);
    _requestProperties();
    _network->bindToSignal(this, SLOT(_onPropertiesChanged(QVariantMap)), "PropertiesChanged", "org.freedesktop.NetworkManager.AccessPoint");
}

int32_t WifiNetwork::id() const
{
    return _id;
}

const QString& WifiNetwork::ssid() const
{
    return _ssid;
}

const QString& WifiNetwork::bssid() const
{
    return _bssid;
}

uint32_t WifiNetwork::quality() const
{
    return _quality;
}

uint32_t WifiNetwork::frequency() const
{
    return _frequency;
}

AccessPointFlags WifiNetwork::flags() const
{
    return _flags;
}

SecurityFlags WifiNetwork::securityFlags() const
{
    return _secFlags;
}

NetworkMode WifiNetwork::mode() const
{
    return _mode;
}

uint32_t WifiNetwork::maxBitrate() const
{
    return _maxBitrate;
}

void WifiNetwork::_requestProperties()
{
    QVariantList args;
    args << QString("org.freedesktop.NetworkManager.AccessPoint");
    QDBusMessage response = _network->call("GetAll", args);

    const QDBusArgument& arg = response.arguments()[0].value<QDBusArgument>();
    arg.beginMap();
    while (!arg.atEnd())
    {
        QString property;
        QVariant value;
        arg.beginMapEntry();
        arg >> property >> value;
        arg.endMapEntry();

        if (property == "Ssid")
            _ssid = qvariant_cast<QString>(value);
        else if (property == "HwAddress")
            _bssid = qvariant_cast<QString>(value);
        else if (property == "Strength")
            _quality = qvariant_cast<uchar>(value);
        else if (property == "Frequency")
            _frequency = qvariant_cast<uint>(value);
        else if (property == "Flags")
            _flags = static_cast<AccessPointFlags>(qvariant_cast<uint>(value));
        else if (property == "WpaFlags")
            _secFlags = static_cast<SecurityFlags>(qvariant_cast<uint>(value));
        else if (property == "Mode")
            _mode = static_cast<NetworkMode>(qvariant_cast<uint>(value));
        else if (property == "MaxBitrate")
            _maxBitrate = qvariant_cast<uint>(value);
    }
    arg.endMap();
}

void WifiNetwork::_onPropertiesChanged(QVariantMap props)
{
    if (props.contains("Strength"))
        _quality = qvariant_cast<uchar>(props.value("Strength"));

    emit propertiesChanged();
}
