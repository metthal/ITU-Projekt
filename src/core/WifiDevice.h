#ifndef WIFI_DEVICE_H
#define WIFI_DEVICE_H

#include <QObject>
#include "NetworkManagerDBusObject.h"

enum DeviceType
{
    NM_DEVICE_TYPE_WIFI = 2
};

class WifiDevice : public QObject
{
    Q_OBJECT

public:
    WifiDevice(const QString& path);
    virtual ~WifiDevice();

    bool isValid();
    QString name();

private slots:
    void _requestNameFinished(QDBusMessage response);

private:
    void _requestName();

    NetworkManagerDBusObject* _device;
    QString _name;
    bool _validated;
    bool _valid;
};

#endif // WIFI_DEVICE_H
