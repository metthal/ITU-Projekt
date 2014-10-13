#include "dummy.h"
#include "WifiManager.h"
#include <iostream>

QString dummyFoo()
{
    WifiManager* mgr = new WifiManager;
    mgr->listDevices();
    return QString("Dummy Foo");
}
