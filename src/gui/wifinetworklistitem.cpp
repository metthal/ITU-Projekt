#include "wifinetworklistitem.h"
#include "ui_wifinetworklistitem.h"

WifiNetworkListItem::WifiNetworkListItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WifiNetworkListItem)
{
    ui->setupUi(this);
    QPixmap  WifiIconPicture("/home/lukas/skola/3BIT/5.sem/ITU/projekt-ITU/gui/rsrc/wifi-icon.png");
    ui->WifiIcon->setPixmap(WifiIconPicture);

    ui->WifiStrength->setText("95%");
    ui->WifiSSID->setText("WifiName");
    ui->WifiHWAddr->setText("20:16:d8:6f:d0:43");
    ui->WifiChan->setText("Chan: 10");

    QPixmap  LockIconPicture("/home/lukas/skola/3BIT/5.sem/ITU/projekt-ITU/gui/rsrc/lock.png");
    ui->LockIcon->setPixmap(LockIconPicture);
    ui->WifiCrypto->setText("WPA2");

    QPixmap  WatchIconPicture("/home/lukas/skola/3BIT/5.sem/ITU/projekt-ITU/gui/rsrc/eye.png");
    ui->WatchIcon->setPixmap(WatchIconPicture);

    ui->WifiFirstSeen->setText("10.10.2014");
    ui->WifiLastSeen->setText("25.10.2014");

}

WifiNetworkListItem::~WifiNetworkListItem()
{
    delete ui;
}
