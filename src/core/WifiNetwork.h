#ifndef WIFI_NETWORK_H
#define WIFI_NETWORK_H

#include <QObject>
#include <QVariant>
#include <QString>
#include <QDateTime>
#include "NetworkManagerDBusObject.h"

enum AccessPointFlags
{
    NM_802_11_AP_FLAGS_NONE             = 0x0,
    NM_802_11_AP_FLAGS_PRIVACY          = 0x1
};

enum SecurityFlags
{
    NM_802_11_AP_SEC_NONE               = 0x0,
    NM_802_11_AP_SEC_PAIR_WEP40         = 0x1,
    NM_802_11_AP_SEC_PAIR_WEP104        = 0x2,
    NM_802_11_AP_SEC_PAIR_TKIP          = 0x4,
    NM_802_11_AP_SEC_PAIR_CCMP          = 0x8,
    NM_802_11_AP_SEC_GROUP_WEP40        = 0x10,
    NM_802_11_AP_SEC_GROUP_WEP104       = 0x20,
    NM_802_11_AP_SEC_GROUP_TKIP         = 0x40,
    NM_802_11_AP_SEC_GROUP_CCMP         = 0x80,
    NM_802_11_AP_SEC_KEY_MGMT_PSK       = 0x100,
    NM_802_11_AP_SEC_KEY_MGMT_802_1X    = 0x200
};

enum NetworkMode
{
    NM_802_11_MODE_UNKNOWN              = 0,
    NM_802_11_MODE_ADHOC                = 1,
    NM_802_11_MODE_INFRA                = 2,
    NM_802_11_MODE_AP                   = 3
};

class WifiNetwork : public QObject
{
    Q_OBJECT

public:
    WifiNetwork(const QString& path);
    WifiNetwork(const QString& ssid, const QString& bssid, const QDateTime& firstSeen, const QDateTime& lastSeen);
    virtual ~WifiNetwork();

    bool operator==(const WifiNetwork& rhs);
    bool operator!=(const WifiNetwork& rhs);

    const QString& SSID() const;
    const QString& BSSID() const;
    uint32_t quality() const;
    uint32_t frequency() const;
    AccessPointFlags flags() const;
    SecurityFlags securityFlags() const;
    NetworkMode mode() const;
    uint32_t maxBitrate() const;

signals:
    void propertiesChanged();

private slots:
    void _onPropertiesChanged(QVariantMap props);

private:
    void _requestProperties();

    NetworkManagerDBusObject* _network;
    QString _ssid;
    QString _bssid;
    uint32_t _quality;
    uint32_t _frequency;
    AccessPointFlags _flags;
    SecurityFlags _secFlags;
    NetworkMode _mode;
    uint32_t _maxBitrate;
};

#endif // WIFI_NETWORK_H
