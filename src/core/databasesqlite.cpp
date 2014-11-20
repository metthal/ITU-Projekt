#include "databasesqlite.h"
#include <QFile>
#include <QSqlQuery>
#include <QMessageBox>
#include "Exception.h"

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

void DatabaseSQLite::open()
{
    if (_db.isOpen())
        return;

    bool exists = QFile::exists(_dbPath);

    // Find QSLite driver
    _db = QSqlDatabase::addDatabase("QSQLITE");
    _db.setDatabaseName(_dbPath);

    // Open database
    if(!_db.open())
        throw Exception("Couldn't open SQLite database.");

    if (!exists)
        createTables();
}

void DatabaseSQLite::close()
{
    if (!_db.isOpen())
        return;

    _db.close();
}

void DatabaseSQLite::remove()
{
    close();
    if(!QFile::remove(_dbPath))
        throw Exception("Couldn't remove SQLite database.");;
}

void DatabaseSQLite::log(WifiNetwork* network)
{
    if (!_db.isOpen())
    {
        throw Exception("Database is closed, can't update networks.");
    }

    QDateTime dt = QDateTime::currentDateTime();

    QString insertQuery = "REPLACE INTO WiFi VALUES(%1,'%2','%3',datetime('%4'),datetime('%5'), %6, %7)";
    int32_t id = network->id();
    insertQuery = insertQuery.arg(id != -1 ? QString::number(id) : "NULL", network->ssid(), network->bssid(), dt.toString(_datetimeFormat), dt.toString(_datetimeFormat), QString::number(network->frequency()), QString::number(network->securityFlags()));
    QSqlQuery query = QSqlQuery(_db);
    if (!query.exec(insertQuery))
    {
        throw Exception("Failed to execute SQLite insert query.");
    }
}

QList<WifiNetwork*> DatabaseSQLite::getNetworks()
{
    if (!_db.isOpen())
    {
        throw Exception("Database is closed, can't retrieve networks.");
    }

    QList<WifiNetwork*> networks;
    QString getQuery = "SELECT * FROM WiFi";
    QSqlQuery query = QSqlQuery(_db);
    if (!query.exec(getQuery))
    {
        throw Exception("Failed to execute SQLite select query.");
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
            uint32_t frequency = query.value(5).toUInt();
            SecurityFlags secFlags = (SecurityFlags)query.value(6).toUInt();
            QString text = "Wifi %1 loaded from DB: %2 %3 (first  %4) (last  %5) %6 %7";
            text = text.arg(QString::number(id), ssid, bssid, firstSeen.toString(_datetimeFormat), lastSeen.toString(_datetimeFormat), QString::number(frequency), QString::number(secFlags));
            networks.append(new WifiNetwork(id, ssid, bssid, firstSeen, lastSeen, frequency, secFlags));
        }
    }
    else
    {
        throw Exception("SQLite select query didn't execute correctly.");
    }
    return networks;
}

void DatabaseSQLite::createTables()
{
    QString createWifiTable =   "CREATE TABLE WiFi "
                                "(id integer PRIMARY KEY, "
                                "ssid VARCHAR(32), "
                                "bssid VARCHAR(17), "
                                "firstSeen DATETIME, "
                                "lastSeen DATETIME,"
                                "frequency INTEGER,"
                                "secFlags INTEGER)";

    QSqlQuery query = QSqlQuery(_db);
    if (!query.exec(createWifiTable))
        throw Exception("Failed to create SQLite table.");
}
