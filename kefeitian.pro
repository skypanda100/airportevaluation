#-------------------------------------------------
#
# Project created by QtCreator 2016-04-25T09:45:22
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = kefeitian
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    flowlayout.cpp \
    database/pgdatabase.cpp \
    bean/airport.cpp \
    bean/qualitycontrolautomatic.cpp \
    bean/qualitycontrolsummary.cpp \
    bean/qualitycontrolsource.cpp

HEADERS  += mainwindow.h \
    flowlayout.h \
    database/pgdatabase.h \
    bean/airport.h \
    bean/qualitycontrolautomatic.h \
    bean/qualitycontrolsummary.h \
    bean/qualitycontrolsource.h

RESOURCES += \
    res.qrc
