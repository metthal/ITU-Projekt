#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dummy.h"

MainWindow::MainWindow(QWidget *parent) :
    KXmlGuiWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


   ui->klistwidget->addItem(test());
}

MainWindow::~MainWindow()
{
    delete ui;
}
