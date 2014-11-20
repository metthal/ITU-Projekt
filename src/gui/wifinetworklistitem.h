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

const unsigned int channel2[14]={2412,2417,2422,2427,2432,2437,2442,2447,2452,2457,2462,2467,2472,2484};

#endif // WIFINETWORKLISTITEM_H
