#ifndef DATABASESQLITE_H
#define DATABASESQLITE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QList>
#include <QDir>

#include "WifiNetwork.h"

class DatabaseSQLite : public QObject
{
    Q_OBJECT

public:
    DatabaseSQLite(QObject *parent = 0);
    ~DatabaseSQLite();

public:
    void open(const QString& path);
    void close();
    void remove();

    void log(WifiNetwork* network);
    QList<WifiNetwork*> getNetworks();

    QSqlError lastError();

private:
    void createTables();

    QString _datetimeFormat;
    QString _dbPath;
    QSqlDatabase _db;
};

#endif // DATABASESQLITE_H
