#include "wifinetworklistitem.h"
#include "ui_wifinetworklistitem.h"
#include <QPixmap>

WifiNetworkListItem::WifiNetworkListItem(WifiNetwork* sourceNetwork, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WifiNetworkListItem),
    network(sourceNetwork)
{
    ui->setupUi(this);
    setStyleSheet("* { background-color: rgb(224,255,255); }");
    ui->WifiSSID->setText(network->ssid());
    ui->WifiHWAddr->setText(network->bssid());
    ui->WifiStrength->setText(QString::number(network->quality()) + "%");
    ui->WifiCrypto->setText(network->secured() ? network->secureMechanism() : "");
    ui->WifiFirstSeen->setText(network->firstSeen().toString("yyyy-MM-dd hh:mm:ss"));
    ui->WifiLastSeen->setText(network->lastSeen().toString("yyyy-MM-dd hh:mm:ss"));

    if (network->quality() == 0)
        ui->WifiIcon->setPixmap(QPixmap("/usr/local/share/WifiMgr/images/wifi0.png"));
    else if ((0 < network->quality()) && (network->quality() <= 20))
        ui->WifiIcon->setPixmap(QPixmap("/usr/local/share/WifiMgr/images/wifi1.png"));
    else if ((21 <= network->quality()) && (network->quality() <= 40))
        ui->WifiIcon->setPixmap(QPixmap("/usr/local/share/WifiMgr/images/wifi2.png"));
    else if ((41 <= network->quality()) && (network->quality() <= 60))
        ui->WifiIcon->setPixmap(QPixmap("/usr/local/share/WifiMgr/images/wifi3.png"));
    else if ((61 <= network->quality()) && (network->quality() <= 80))
        ui->WifiIcon->setPixmap(QPixmap("/usr/local/share/WifiMgr/images/wifi4.png"));
    else if ((81 <= network->quality()) && (network->quality() <= 100))
        ui->WifiIcon->setPixmap(QPixmap("/usr/local/share/WifiMgr/images/wifi5.png"));

    if (network->secured())
        ui->LockIcon->setPixmap(QPixmap("/usr/local/share/WifiMgr/images/lock.png"));
    else
        ui->LockIcon->setPixmap(QPixmap("/usr/local/share/WifiMgr/images/un_lock.png"));

    if (network->maxBitrate() <= 11000)
        ui->BitrateIcon->setText("B");
    if ((network->maxBitrate() > 11000) && (network->maxBitrate() <= 54000))
        ui->BitrateIcon->setText("G");
    if(network->maxBitrate() > 54000)
        ui->BitrateIcon->setText("N");

    ui->WifiChan->setText(QString::number(network->channel()));

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
