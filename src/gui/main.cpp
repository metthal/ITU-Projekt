#include "mainwindow.h"
#include <KCmdLineArgs>
#include <KAboutData>
#include <KApplication>

int main(int argc, char *argv[])
{
    KAboutData aboutData("WifiMgr", 0,
          ki18n("WiFi Manager"), "0.1",
          ki18n("Manager and data collector for WiFi networks"),
          KAboutData::License_GPL,
          ki18n("2014"));
      KCmdLineArgs::init(argc, argv, &aboutData);

    KApplication app;
    MainWindow* window = new MainWindow();
    window->show();

    return app.exec();
}
