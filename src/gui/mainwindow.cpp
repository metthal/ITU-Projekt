#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "wifinetworklistitem.h"
#include <kmessagebox.h>

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::_init()
{
    mgr = new WifiManager;
    mgr->loadDevices();

    if (mgr->devices().empty())
    {
        KMessageBox::error(this, "No wireless devices found.");
        return;
    }

    mgr->loadNetworks(mgr->devices()[0]);
    for (WifiNetwork* network : mgr->networks())
        connect(network, SIGNAL(propertiesChanged()), this, SLOT(onPropertyChanged()));

    _orderItems();
}

void MainWindow::onPropertyChanged()
{
    _orderItems();
}

void MainWindow::_orderItems()
{
    int32_t count = ui->networkList->count();
    for (int32_t i = 0; i < count; ++i)
        delete ui->networkList->takeItem(0);

    QList<WifiNetwork*> networkList(mgr->networks());
    std::sort(networkList.begin(), networkList.end(), [](WifiNetwork* lhs, WifiNetwork* rhs)
            {
                return (*lhs > *rhs);
            });

    for (WifiNetwork* network : networkList)
    {
        QListWidgetItem* newItem = new QListWidgetItem(ui->networkList);
        WifiNetworkListItem* newWifiItem = new WifiNetworkListItem(network, this);
        newItem->setSizeHint(QSize(0, 100));
        ui->networkList->addItem(newItem);
        ui->networkList->setItemWidget(newItem, newWifiItem);
    }
}
