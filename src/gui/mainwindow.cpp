#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "wifinetworklistitem.h"
#include <kmessagebox.h>
#include <QScrollBar>
#include <QFileDialog>
#include <QDir>
#include "Exception.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _init();
}

MainWindow::~MainWindow()
{
    mgr->storeDatabase(currentDbPath);
    delete ui;
    delete mgr;
}

void MainWindow::_init()
{
    currentDbPath = QDir::homePath() + "/WifiMgr.sqlite";

    mgr = new WifiManager();
    try
    {
        mgr->loadDatabase(currentDbPath);
    }
    catch(Exception& e)
    {
        KMessageBox::error(this, "Database failed to open.");
    }
    mgr->loadDevices();

    if (mgr->devices().empty())
        KMessageBox::error(this, "No wireless devices found.");
    else
        mgr->loadNetworks(mgr->devices()[0]);

    for (WifiNetwork* network : mgr->networks())
        connect(network, SIGNAL(propertiesChanged()), this, SLOT(onPropertyChanged()));

    ui->networkList->setStyleSheet("KListWidget { background-color: rgb(176,224,230); }");

    _orderItems();
}

void MainWindow::onPropertyChanged()
{
    _orderItems();
}

void MainWindow::_orderItems()
{
    int scrollValue = ui->networkList->verticalScrollBar()->sliderPosition();

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
        newItem->setSizeHint(QSize(0, 130));
        ui->networkList->addItem(newItem);
        ui->networkList->setItemWidget(newItem, newWifiItem);
    }

    ui->networkList->verticalScrollBar()->setSliderPosition(scrollValue);
}

void MainWindow::on_actionOpen_triggered()
{
    mgr->storeDatabase(currentDbPath);
    WifiManager* newMgr = new WifiManager();
    QString newDbPath = QFileDialog::getOpenFileName(this, "Open database", QDir::homePath(), "Database Files (*.sqlite)");
    try
    {
        newMgr->loadDatabase(newDbPath);
        newMgr->loadDevices();

        if (newMgr->devices().empty())
            throw Exception("No wireless devices found.");
        else
            newMgr->loadNetworks(mgr->devices()[0]);

        currentDbPath = newDbPath;

        // Disconnect all current signals
        disconnect(this, SLOT(onPropertyChanged()));
        // Reconnect new signals
        for (WifiNetwork* network : newMgr->networks())
            connect(network, SIGNAL(propertiesChanged()), this, SLOT(onPropertyChanged()));
    }
    catch(Exception& e)
    {
        KMessageBox::error(this, "Database failed to open.");
    }
}
