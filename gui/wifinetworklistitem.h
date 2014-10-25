#ifndef WIFINETWORKLISTITEM_H
#define WIFINETWORKLISTITEM_H

#include <QWidget>

namespace Ui {
class WifiNetworkListItem;
}

class WifiNetworkListItem : public QWidget
{
    Q_OBJECT

public:
    explicit WifiNetworkListItem(QWidget *parent = 0);
    ~WifiNetworkListItem();

private:
    Ui::WifiNetworkListItem *ui;
};

#endif // WIFINETWORKLISTITEM_H
