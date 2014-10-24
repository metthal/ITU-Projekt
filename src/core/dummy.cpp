#include "dummy.h"
#include "WifiManager.h"
#include "databasesqlite.h"
#include <iostream>

QString dummyFoo()
{
    WifiManager* mgr = new WifiManager;
    DatabaseSQLite* db = new DatabaseSQLite;
    db->open();
    db->getNetworks();
    mgr->listDevices();
    if (mgr->devices().count() > 0)
    {
        WifiDevice *device = mgr->devices().first();
        device->listNetworks();
        if (device->networks().count() > 0)
        {
            WifiNetwork *netw = device->networks().first();
            db->log(netw);
        }
    }
    db->close();
    return QString("Dummy Foo");
}
