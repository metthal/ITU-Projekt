#include "dummy.h"
#include "WifiManager.h"

QString dummyFoo()
{
    WifiManager* mgr = new WifiManager;
    mgr->listDevices();
    return QString("Dummy Foo");

}
