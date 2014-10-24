#ifndef DATABASESQLITE_H
#define DATABASESQLITE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QList>

#include "WifiNetwork.h"

class DatabaseSQLite : public QObject
{
    Q_OBJECT

public:
    DatabaseSQLite(QObject *parent = 0);
    ~DatabaseSQLite();

public:
    bool open();
    bool close();
    bool remove();

    bool log(WifiNetwork* network);
    QList<WifiNetwork*> getNetworks();

    QSqlError lastError();

private:
    bool createTables();

    QString datetimeFormat;
    QString dbPath;
    QSqlDatabase db;
};

#endif // DATABASESQLITE_H
