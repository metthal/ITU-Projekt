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

QDBusMessage NetworkManagerDBusObject::call(const QString& method, const QVariantList& args)
{
    return _interface->callWithArgumentList(QDBus::BlockWithGui, method, args);
}

void NetworkManagerDBusObject::bindToSignal(QObject* receiver, const char* slot, const QString& signal, const QString& interface)
{
    QDBusConnection::systemBus().connect(networkManagerService, _interface->path(), interface, signal, receiver, slot);
}

