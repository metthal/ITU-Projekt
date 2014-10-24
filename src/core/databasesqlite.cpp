#include "databasesqlite.h"
#include <QFile>
#include <QDir>
#include <QSqlQuery>
#include <QMessageBox>

DatabaseSQLite::DatabaseSQLite(QObject *parent)
{
    // Set path to database
    dbPath = QString(QDir::home().path());
    dbPath.append(QDir::separator()).append("wifimgr.sqlite");
    dbPath = QDir::toNativeSeparators(dbPath);
    datetimeFormat = "yyyy-MM-dd hh:mm:ss";
}

DatabaseSQLite::~DatabaseSQLite()
{
    close();
}

bool DatabaseSQLite::open()
{
    if (db.isOpen())
        return true;

    bool exists = QFile::exists(dbPath);

    // Find QSLite driver
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);

    // Open database
    bool opened = db.open();

    if (!exists)
        createTables();

    return opened;
}

bool DatabaseSQLite::close()
{
    if (!db.isOpen())
        return true;

    db.close();
    return true;
}

bool DatabaseSQLite::remove()
{
    close();
    return QFile::remove(dbPath);
}

bool DatabaseSQLite::log(WifiNetwork* network)
{
    if (!db.isOpen())
    {
        // TODO: Error, DB not open
    }

    QDateTime dt = QDateTime::currentDateTime();

    QString insertQuery = "INSERT INTO WiFi VALUES(NULL,'%1','%2',datetime('%3'),datetime('%4'))";
    insertQuery = insertQuery.arg(network->SSID(), network->BSSID(), dt.toString(datetimeFormat), dt.toString(datetimeFormat));
    QMessageBox::information(NULL,"",insertQuery);
    QSqlQuery query = QSqlQuery(insertQuery, db);
    if (!query.exec())
    {
        QMessageBox::information(NULL,"",QString("Inser error: ").arg(query.lastError().text()));
        //TODO: throw - failed query
    }
    return true;
}

QList<WifiNetwork*> DatabaseSQLite::getNetworks()
{
    if (!db.isOpen())
    {
        // TODO: Error, DB not open
    }

    QList<WifiNetwork*> networks;
    QString getQuery = "SELECT * FROM WiFi";
    QSqlQuery query = QSqlQuery(getQuery, db);
    if (!query.exec())
    {
        QMessageBox::information(NULL,"",QString("Select error: %1").arg(query.lastError().text()));
        //TODO: throw - failed query
    }
    int i = 0;
    if (query.isActive())
    {
        while (query.next())
        {
            QString ssid = query.value(1).toString();
            QString bssid = query.value(2).toString();
            QDateTime firstSeen = query.value(3).toDateTime();
            QDateTime lastSeen = query.value(4).toDateTime();
            QString text = "Wifi %1 loaded from DB: %2 %3 (first  %4) (last  %5)";
            text = text.arg(QString::number(i++), ssid, bssid, firstSeen.toString(datetimeFormat), lastSeen.toString(datetimeFormat));
            QMessageBox::information(NULL,"",text);
            networks.append(new WifiNetwork(ssid, bssid, firstSeen, lastSeen));
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

    QSqlQuery query = QSqlQuery(createWifiTable, db);
    if (!query.exec())
    {
        QMessageBox::information(NULL,"",QString("Create table error: %1").arg(query.lastError().text()));
        //TODO: throw - failed query
    }
    return true;
}
