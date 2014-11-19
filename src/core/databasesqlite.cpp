#include "databasesqlite.h"
#include <QFile>
#include <QSqlQuery>
#include <QMessageBox>

DatabaseSQLite::DatabaseSQLite(const QString& path, QObject *parent)
{
    // Set path to database
    _dbPath = QDir::toNativeSeparators(path);
    _datetimeFormat = "yyyy-MM-dd hh:mm:ss";
}

DatabaseSQLite::~DatabaseSQLite()
{
    close();
}

bool DatabaseSQLite::open()
{
    if (_db.isOpen())
        return true;

    bool exists = QFile::exists(_dbPath);

    // Find QSLite driver
    _db = QSqlDatabase::addDatabase("QSQLITE");
    _db.setDatabaseName(_dbPath);

    // Open database
    bool opened = _db.open();

    if (!exists)
        createTables();

    return opened;
}

bool DatabaseSQLite::close()
{
    if (!_db.isOpen())
        return true;

    _db.close();
    return true;
}

bool DatabaseSQLite::remove()
{
    close();
    return QFile::remove(_dbPath);
}

bool DatabaseSQLite::log(WifiNetwork* network)
{
    if (!_db.isOpen())
    {
        // TODO: Error, DB not open
    }

    QDateTime dt = QDateTime::currentDateTime();

    QString insertQuery = "REPLACE INTO WiFi VALUES(%1,'%2','%3',datetime('%4'),datetime('%5'))";
    int32_t id = network->id();
        insertQuery = insertQuery.arg(id != -1 ? QString::number(id) : "NULL", network->ssid(), network->bssid(), dt.toString(_datetimeFormat), dt.toString(_datetimeFormat));
    QSqlQuery query = QSqlQuery(_db);
    if (!query.exec(insertQuery))
    {
        //TODO: throw - failed query
    }
    return true;
}

QList<WifiNetwork*> DatabaseSQLite::getNetworks()
{
    if (!_db.isOpen())
    {
        // TODO: Error, DB not open
    }

    QList<WifiNetwork*> networks;
    QString getQuery = "SELECT * FROM WiFi";
    QSqlQuery query = QSqlQuery(_db);
    if (!query.exec(getQuery))
    {
        //TODO: throw - failed query
    }
    if (query.isActive())
    {
        while (query.next())
        {
            int32_t id = query.value(0).toInt();
            QString ssid = query.value(1).toString();
            QString bssid = query.value(2).toString();
            QDateTime firstSeen = query.value(3).toDateTime();
            QDateTime lastSeen = query.value(4).toDateTime();
            QString text = "Wifi %1 loaded from DB: %2 %3 (first  %4) (last  %5)";
            text = text.arg(QString::number(id), ssid, bssid, firstSeen.toString(_datetimeFormat), lastSeen.toString(_datetimeFormat));
            networks.append(new WifiNetwork(id, ssid, bssid, firstSeen, lastSeen));
        }
    }
    else
    {
        // TODO: Error failed query
    }
    return networks;
}

bool DatabaseSQLite::createTables()
{
    QString createWifiTable =   "CREATE TABLE WiFi "
                                "(id integer PRIMARY KEY, "
                                "ssid VARCHAR(32), "
                                "bssid VARCHAR(17), "
                                "firstSeen DATETIME, "
                                "lastSeen DATETIME)";

    QSqlQuery query = QSqlQuery(_db);
    query.exec(createWifiTable);
    return true;
}
