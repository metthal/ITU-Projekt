#include "wifinetworklistitem.h"
#include "ui_wifinetworklistitem.h"
#include <QPixmap>

static unsigned int channel2[14]={2412,2417,2422,2427,2432,2437,2442,2447,2452,2457,2462,2467,2472,2484};

WifiNetworkListItem::WifiNetworkListItem(WifiNetwork* sourceNetwork, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WifiNetworkListItem),
    network(sourceNetwork)
{
    ui->setupUi(this);
    setStyleSheet("* { background-color: rgb(224,255,255); }");
    ui->WifiSSID->setText(network->ssid());
    ui->WifiHWAddr->setText(network->bssid());
    ui->WifiStrength->setText(QString::number(network->quality())+"%");
    ui->WifiCrypto->setText("Crypto: WPA2"); // ?? will be here?
    ui->WifiFirstSeen->setText("10.10.2014");
    ui->WifiLastSeen->setText("25.10.2014");

    if( network->quality() == 0)
        ui->WifiIcon->setPixmap(QPixmap("/usr/local/share/WifiMgr/images/wifi0.png"));
    if((0 < network->quality()) && (network->quality() <= 20))
        ui->WifiIcon->setPixmap(QPixmap("/usr/local/share/WifiMgr/images/wifi1.png"));
    if((21 <= network->quality()) && (network->quality() <= 40))
        ui->WifiIcon->setPixmap(QPixmap("/usr/local/share/WifiMgr/images/wifi2.png"));
    if((41 <= network->quality()) && (network->quality() <= 60))
        ui->WifiIcon->setPixmap(QPixmap("/usr/local/share/WifiMgr/images/wifi3.png"));
    if((61 <= network->quality()) && (network->quality() <= 80))
        ui->WifiIcon->setPixmap(QPixmap("/usr/local/share/WifiMgr/images/wifi4.png"));
    if((81 <= network->quality()) && (network->quality() <= 100))
        ui->WifiIcon->setPixmap(QPixmap("/usr/local/share/WifiMgr/images/wifi5.png"));

    if(network->securityFlags() > 0x0)
        ui->LockIcon->setPixmap(QPixmap("/usr/local/share/WifiMgr/images/un_lock.png"));
    if(network->securityFlags() == 0x0)
        ui->LockIcon->setPixmap(QPixmap("/usr/local/share/WifiMgr/images/lock.png"));

    // place holder here.
    ui->WatchIcon->setPixmap(QPixmap("/usr/local/share/WifiMgr/images/watch.png"));

    ui->WifiChan->setText(QString::number(GetWifiChannel()));

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

int WifiNetworkListItem::GetWifiChannel()
{
    int len = sizeof(channel2)/sizeof(channel2[0]);
    for (int i = 0; i < len; ++i)
    {
        if (channel2[i] == network->frequency())
            return i;
    }
    return -1;
}

WifiNetworkListItem::~WifiNetworkListItem()
{
    delete ui;
}
