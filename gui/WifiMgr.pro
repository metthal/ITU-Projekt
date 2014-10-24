#-------------------------------------------------
#
# Project created by QtCreator 2014-10-10T08:36:03
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WifiMgr
TEMPLATE = app


SOURCES += ../src/gui/main.cpp\
           ../src/gui/mainwindow.cpp \
    ../src/core/dummy.cpp \
    ../src/core/NetworkManagerDBusObject.cpp \
    ../src/core/WifiDevice.cpp \
    ../src/core/WifiManager.cpp \
    ../src/core/WifiNetwork.cpp \
    ../src/gui/moc_mainwindow.cpp \
    ../src/gui/moc_NetworkManagerDBusObject.cpp \
    ../src/gui/moc_WifiDevice.cpp \
    ../src/gui/moc_WifiManager.cpp \
    ../src/gui/WifiMgr_automoc.cpp \
    ../src/core/databasesqlite.cpp

HEADERS  += ../src/gui/mainwindow.h \
    ../src/core/dummy.h \
    ../src/core/Exception.h \
    ../src/core/NetworkManagerDBusObject.h \
    ../src/core/WifiDevice.h \
    ../src/core/WifiManager.h \
    ../src/core/WifiNetwork.h \
    ../src/gui/ui_mainwindow.h \
    ../src/core/databasesqlite.h

FORMS    += mainwindow.ui
