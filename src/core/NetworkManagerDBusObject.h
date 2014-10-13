#ifndef NETWORK_MANAGER_DBUS_OBJECT_H
#define NETWORK_MANAGER_DBUS_OBJECT_H

#include <QObject>
#include <QDBusInterface>
#include <QDBusMessage>

class NetworkManagerDBusObject : public QObject
{
    Q_OBJECT

public:
    NetworkManagerDBusObject(const QString& path);
    virtual ~NetworkManagerDBusObject();

    QDBusMessage call(const QString& method, const QVariantList& args = QVariantList());
    void bindToSignal(QObject* receiver, const char* slot, const QString& signal, const QString& interface);

signals:
    void responseReceived(QDBusMessage response);

private:
    QDBusInterface* _interface;
};

#endif // NETWORK_MANAGER_DBUS_OBJECT_H
