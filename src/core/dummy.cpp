#include "dummy.h"
#include "WifiManager.h"
#include <iostream>

QString dummyFoo()
{
    WifiManager* mgr = new WifiManager;
    mgr->listDevices();
    if (mgr->_devices.empty())
        std::cout << "NO WIFI INTERFACES FOUND" << std::endl;
    else
        mgr->_devices[0]->listNetworks();
    return QString("Dummy Foo");
}
