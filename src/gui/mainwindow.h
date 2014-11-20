#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "WifiManager.h"

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

private:
    void _init();
    void _orderItems();

    QString currentDbPath;
    Ui::MainWindow* ui;
    WifiManager* mgr;
};

#endif // MAINWINDOW_H
