#include "NetworkManagerDBusObject.h"

static QString networkManagerService = "org.freedesktop.NetworkManager";

NetworkManagerDBusObject::NetworkManagerDBusObject(const QString& path)
    : _interface(new QDBusInterface(networkManagerService, path, QString(), QDBusConnection::systemBus()))
{
}

NetworkManagerDBusObject::~NetworkManagerDBusObject()
{
    if (_interface)
        delete _interface;
}

void NetworkManagerDBusObject::call(const QString& method, const QList<QVariant>& args)
{
    QDBusMessage response = _interface->callWithArgumentList(QDBus::BlockWithGui, method, args);
    emit responseReceived(response);
}

QDBusMessage NetworkManagerDBusObject::directCall(const QString& method, const QList<QVariant>& args)
{
    return _interface->callWithArgumentList(QDBus::BlockWithGui, method, args);
}

void NetworkManagerDBusObject::bindSlotToSignal(QObject* receiver, const char* slot, const QString& signal, const QString& interface)
{
    QDBusConnection::systemBus().connect(networkManagerService, _interface->path(), interface, signal, receiver, slot);
}

