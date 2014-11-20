#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "wifinetworklistitem.h"
#include <kmessagebox.h>
#include <QScrollBar>
#include <QFileDialog>
#include <QDir>
#include "Exception.h"
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
    mgr->storeDatabase(currentDbPath);
    delete ui;
    delete mgr;
}

void MainWindow::_init()
{
    _db = new DatabaseSQLite(this);
    currentDbPath = QDir::homePath() + "/WifiMgr.sqlite";

    mgr = new WifiManager(_db);
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

    _showOOR = true;
    ui->actionToggle_OOR->setText("Hide unavailable");

    _orderItems();
}

void MainWindow::onPropertyChanged()
{
    _orderItems();
}

void MainWindow::_orderItems()
{
    int maximum = ui->networkList->verticalScrollBar()->maximum();
    int scrollValue = ui->networkList->verticalScrollBar()->value();

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
        if (_showOOR || network->quality() > 0)
        {
            QListWidgetItem* newItem = new QListWidgetItem(ui->networkList);
            WifiNetworkListItem* newWifiItem = new WifiNetworkListItem(network, this);
            newItem->setSizeHint(QSize(0, 130));

            ui->networkList->addItem(newItem);
            ui->networkList->setItemWidget(newItem, newWifiItem);
        }
    }

    ui->networkList->verticalScrollBar()->setMaximum(maximum);
    ui->networkList->verticalScrollBar()->setValue(scrollValue);
}

void MainWindow::on_actionOpen_triggered()
{
    QString newDbPath = QFileDialog::getOpenFileName(this, "Open database", QDir::homePath(), "Database Files (*.sqlite)");
    if (newDbPath == "")
        return;

    mgr->storeDatabase(currentDbPath);
    WifiManager* newMgr = new WifiManager(_db);

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

void MainWindow::on_actionToggle_OOR_triggered()
{
    _showOOR = !_showOOR;
    if (_showOOR)
        ui->actionToggle_OOR->setText("Hide unavailable");
    else
        ui->actionToggle_OOR->setText("Show unavailable");
    _orderItems();
}

void MainWindow::on_actionWifi_Manager_triggered()
{
    KMessageBox::about(this, "WiFi manager is designed <a href='http://www.google.com'>by</a>", "About WiFi Manager", KMessageBox::Notify | KMessageBox::AllowLink);
}
