#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QTimer>
#include "WifiManager.h"
#include "databasesqlite.h"
#include "keyEventFilter.h"

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
    void runSearch(const QString& searchString);

private slots:
    void on_actionOpen_triggered();

    void on_actionToggle_OOR_triggered();

    void on_actionWifi_Manager_triggered();

private:
    void _init();
    void _orderItems();
    bool searchInString(const QString& where, const QString& what);
    QListWidgetItem* findWifiItem(const WifiNetwork* wifiToFind);

    QString currentDbPath;
    Ui::MainWindow* ui;
    WifiManager* mgr;
    DatabaseSQLite* _db;
    bool _showOOR;

    QList<WifiNetwork*> _orderedNetworks;

    KeyEventFilter* filter;
};

#endif // MAINWINDOW_H
