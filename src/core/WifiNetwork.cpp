#include "WifiNetwork.h"
#include <QDBusArgument>

WifiNetwork::WifiNetwork(const QString& path)
{
    _network = new NetworkManagerDBusObject(path);
    _requestProperties();
    _network->bindToSignal(this, SLOT(propertiesChanged(QVariantMap)), "PropertiesChanged", "org.freedesktop.NetworkManager.AccessPoint");
}

WifiNetwork::~WifiNetwork()
{
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
        else if (property == "Strength")
            _quality = qvariant_cast<uchar>(value);
        else if (property == "Frequency")
            _frequency = qvariant_cast<uint>(value);
    }
    arg.endMap();
}

void WifiNetwork::propertiesChanged(QVariantMap props)
{
    if (props.contains("Strength"))
        _quality = qvariant_cast<uchar>(props.value("Strength"));
}
