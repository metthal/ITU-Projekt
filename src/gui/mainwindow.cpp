#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "wifinetworklistitem.h"

#include "dummy.h"

MainWindow::MainWindow(QWidget *parent) :
    KXmlGuiWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QListWidgetItem* item = new QListWidgetItem(ui->klistwidget);
    QListWidgetItem* item1 = new QListWidgetItem(ui->klistwidget);
    WifiNetworkListItem* wifiItem = new WifiNetworkListItem(this);
    WifiNetworkListItem* wifiItem1 = new WifiNetworkListItem(this);
    item->setSizeHint(QSize(0,100));
    item1->setSizeHint(QSize(0,100));
    ui->klistwidget->addItem(item);
    ui->klistwidget->addItem(item1);
    ui->klistwidget->setItemWidget(item, wifiItem);
    ui->klistwidget->setItemWidget(item1, wifiItem1);
}

MainWindow::~MainWindow()
{
    delete ui;
}
