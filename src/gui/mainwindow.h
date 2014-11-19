#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "WifiManager.h"
#include "databasesqlite.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void onPropertyChanged();

private:
    void _init();
    void _orderItems();

    Ui::MainWindow* ui;
    WifiManager* mgr;
    DatabaseSQLite* db;
};

#endif // MAINWINDOW_H
