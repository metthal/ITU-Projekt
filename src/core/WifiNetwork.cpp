#include "WifiNetwork.h"
#include <QDBusArgument>
#include <unordered_map>

static std::unordered_map<uint32_t, uint32_t> freqToChanTable =
{
    { 2412, 1 },
    { 2417, 2 },
    { 2422, 3 },
    { 2427, 4 },
    { 2432, 5 },
    { 2437, 6 },
    { 2442, 7 },
    { 2447, 8 },
    { 2452, 9 },
    { 2457, 10 },
    { 2462, 11 },
    { 2467, 12 },
    { 2472, 13 },
    { 2484, 14 },
    { 4915, 183 },
    { 4920, 184 },
    { 4925, 185 },
    { 4935, 187 },
    { 4940, 188 },
    { 4945, 189 },
    { 4960, 192 },
    { 4980, 196 },
    { 5035, 7 },
    { 5040, 8 },
    { 5045, 9 },
    { 5055, 11 },
    { 5060, 12 },
    { 5080, 16 },
    { 5170, 34 },
    { 5180, 36 },
    { 5190, 38 },
    { 5200, 40 },
    { 5210, 42 },
    { 5220, 44 },
    { 5230, 46 },
    { 5240, 48 },
    { 5260, 52 },
    { 5280, 56 },
    { 5300, 60 },
    { 5320, 64 },
    { 5500, 100 },
    { 5520, 104 },
    { 5540, 108 },
    { 5560, 112 },
    { 5580, 116 },
    { 5600, 120 },
    { 5620, 124 },
    { 5640, 128 },
    { 5660, 132 },
    { 5680, 136 },
    { 5700, 140 },
    { 5745, 149 },
    { 5765, 153 },
    { 5785, 157 },
    { 5805, 161 },
    { 5825, 165 }
};

WifiNetwork::WifiNetwork(const QString& path)
{
    _id = -1;
    _network = nullptr;
    _quality = 0;
    assignToObject(path);
    _firstSeen = QDateTime::currentDateTime();
}

WifiNetwork::WifiNetwork(int32_t id, const QString& ssid, const QString& bssid, const QDateTime& firstSeen, const QDateTime& lastSeen, const uint32_t frequency, const SecurityFlags wpaFlags, const SecurityFlags rsnFlags)
{
    _network = nullptr;
    _id = id;
    _ssid = QString(ssid);
    _bssid = QString(bssid);
    _quality = 0;
    _frequency = frequency;
    _flags = NM_802_11_AP_FLAGS_NONE;
    _wpaFlags = wpaFlags;
    _rsnFlags = rsnFlags;
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

SecurityFlags WifiNetwork::wpaFlags() const
{
    return _wpaFlags;
}

SecurityFlags WifiNetwork::rsnFlags() const
{
    return _rsnFlags;
}

NetworkMode WifiNetwork::mode() const
{
    return _mode;
}

uint32_t WifiNetwork::maxBitrate() const
{
    return _maxBitrate;
}

uint32_t WifiNetwork::channel() const
{
    std::unordered_map<uint32_t, uint32_t>::iterator itr = freqToChanTable.find(_frequency);
    if (itr == freqToChanTable.end())
        return 0;

    return (*itr).second;
}

bool WifiNetwork::secured() const
{
    return ((_wpaFlags != NM_802_11_AP_SEC_NONE) && (_rsnFlags != NM_802_11_AP_SEC_NONE));
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
            _wpaFlags = static_cast<SecurityFlags>(qvariant_cast<uint>(value));
        else if (property == "RsnFlags")
            _rsnFlags = static_cast<SecurityFlags>(qvariant_cast<uint>(value));
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
