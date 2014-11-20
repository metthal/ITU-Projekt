#ifndef WIFINETWORKLISTITEM_H
#define WIFINETWORKLISTITEM_H

#include <QWidget>
#include "WifiNetwork.h"

namespace Ui {
class WifiNetworkListItem;
}

class WifiNetworkListItem : public QWidget
{
    Q_OBJECT

public:
    explicit WifiNetworkListItem(WifiNetwork* sourceNetwork, QWidget *parent = 0);
    ~WifiNetworkListItem();

    WifiNetwork* wifiNetwork();
    int GetWifiChannel();

public slots:
    void onPropertyChanged();

private:
    Ui::WifiNetworkListItem* ui;
    WifiNetwork* network;
};

#endif // WIFINETWORKLISTITEM_H
