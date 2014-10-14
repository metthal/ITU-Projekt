#ifndef WIFI_NETWORK_H
#define WIFI_NETWORK_H

#include <QObject>
#include <QVariant>
#include "NetworkManagerDBusObject.h"

class WifiNetwork : public QObject
{
    Q_OBJECT

public:
    WifiNetwork(const QString& path);
    virtual ~WifiNetwork();

signals:
    void propertiesChanged();

private slots:
    void _onPropertiesChanged(QVariantMap props);

private:
    void _requestProperties();

    NetworkManagerDBusObject* _network;
    QString _ssid;
    uint32_t _quality;
    uint32_t _frequency;
};

#endif // WIFI_NETWORK_H
