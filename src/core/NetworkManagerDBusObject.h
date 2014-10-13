#ifndef NETWORK_MANAGER_DBUS_OBJECT_H
#define NETWORK_MANAGER_DBUS_OBJECT_H

#include <QObject>
#include <QDBusInterface>

class NetworkManagerDBusObject : public QObject
{
    Q_OBJECT

public:
    NetworkManagerDBusObject(const QString& path);
    virtual ~NetworkManagerDBusObject();

    void call(const QString& method, const QList<QVariant>& args = QList<QVariant>());
    QDBusMessage directCall(const QString& method, const QList<QVariant>& args = QList<QVariant>());

    void bindSlotToSignal(QObject* receiver, const char* slot, const QString& signal, const QString& interface);

signals:
    void responseReceived(QDBusMessage response);

private:
    QDBusInterface* _interface;
};

#endif // NETWORK_MANAGER_DBUS_OBJECT_H
