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

private slots:
    void on_actionOpen_triggered();

    void on_actionToggle_OOR_triggered();

    void on_actionWifi_Manager_triggered();

private:
    void _init();
    void _orderItems();

    QString currentDbPath;
    Ui::MainWindow* ui;
    WifiManager* mgr;
    DatabaseSQLite* _db;
    bool _showOOR;
};

#endif // MAINWINDOW_H
