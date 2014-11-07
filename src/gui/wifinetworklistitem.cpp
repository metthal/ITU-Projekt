#include "wifinetworklistitem.h"
#include "ui_wifinetworklistitem.h"
#include <QPixmap>

WifiNetworkListItem::WifiNetworkListItem(WifiNetwork* sourceNetwork, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WifiNetworkListItem),
    network(sourceNetwork)
{
    ui->setupUi(this);

    ui->WifiSSID->setText(network->ssid());
    ui->WifiHWAddr->setText(network->bssid());
    ui->WifiChan->setText(QString::number(network->frequency()));
    ui->WifiStrength->setText(QString::number(network->quality()));
    ui->WifiCrypto->setText("WPA2");
    ui->WifiFirstSeen->setText("10.10.2014");
    ui->WifiLastSeen->setText("25.10.2014");

    ui->WifiIcon->setPixmap(QPixmap("/home/lukas/skola/3BIT/5.sem/ITU/projekt-ITU/gui/rsrc/wifi-icon.png"));
    ui->LockIcon->setPixmap(QPixmap("/home/lukas/skola/3BIT/5.sem/ITU/projekt-ITU/gui/rsrc/lock.png"));
    ui->WatchIcon->setPixmap(QPixmap("/home/lukas/skola/3BIT/5.sem/ITU/projekt-ITU/gui/rsrc/eye.png"));

    connect(sourceNetwork, SIGNAL(propertiesChanged()), this, SLOT(onPropertyChanged()));
}

WifiNetwork* WifiNetworkListItem::wifiNetwork()
{
    return network;
}

void WifiNetworkListItem::onPropertyChanged()
{
    ui->WifiStrength->setText(QString::number(network->quality()));
}

WifiNetworkListItem::~WifiNetworkListItem()
{
    delete ui;
}
