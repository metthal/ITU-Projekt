#include "wifinetworklistitem.h"
#include "ui_wifinetworklistitem.h"

WifiNetworkListItem::WifiNetworkListItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WifiNetworkListItem)
{
    ui->setupUi(this);
}

WifiNetworkListItem::~WifiNetworkListItem()
{
    delete ui;
}
