#include "dummy.h"
#include "WifiManager.h"
#include "databasesqlite.h"
#include <iostream>

QString dummyFoo()
{
    WifiManager* mgr = new WifiManager;
    mgr->loadDevices();
    const QList<WifiDevice*>& devices = mgr->devices();
    for (auto device : devices)
        std::cout << device->name().toStdString() << std::endl;

    QString dbPath = QDir::homePath().append(QDir::separator()).append("wifimgr.sqlite");
    mgr->loadDatabase(dbPath);
    for (auto network : mgr->networks())
        std::cout << network->ssid().toStdString() << " " << network->quality() << std::endl;

    mgr->loadNetworks(devices[0]);
    std::cout << std::endl;

    for (auto network : mgr->networks())
        std::cout << network->ssid().toStdString() << " " << network->quality() << std::endl;

    mgr->storeDatabase(dbPath);
    return QString("Dummy Foo");
}

QListWidgetItem test()
{
    return QListWidgetItem("test");
}
