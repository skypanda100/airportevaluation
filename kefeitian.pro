#-------------------------------------------------
#
# Project created by QtCreator 2016-04-25T09:45:22
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = kefeitian
TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    database/pgdatabase.cpp \
    bean/airport.cpp \
    bean/qualitycontrolautomatic.cpp \
    bean/qualitycontrolsummary.cpp \
    bean/qualitycontrolsource.cpp \
    sjdrinputwidget.cpp \
    sjdrmainwidget.cpp \
    database/asdatabase.cpp \
    model/treeitem.cpp \
    model/treemodel.cpp \
    control/sjdrcontrol.cpp \
    sjdrqualitywidget.cpp \
    kfttjresultwidget.cpp \
    bean/monthsummary.cpp \
    bean/extremum.cpp \
    model/tablemodel.cpp \
    control/kfttjcontrol.cpp \
    qchartviewer.cpp \
    fmginputwidget.cpp \
    fmgresultwidget.cpp \
    control/fmgcontrol.cpp \
    rckqinputwidget.cpp

HEADERS  += mainwindow.h \
    database/pgdatabase.h \
    bean/airport.h \
    bean/qualitycontrolautomatic.h \
    bean/qualitycontrolsummary.h \
    bean/qualitycontrolsource.h \
    sjdrinputwidget.h \
    sjdrmainwidget.h \
    database/asdatabase.h \
    model/treeitem.h \
    model/treemodel.h \
    control/sjdrcontrol.h \
    control/ctlcons.h \
    sjdrqualitywidget.h \
    kfttjresultwidget.h \
    bean/monthsummary.h \
    bean/extremum.h \
    model/tablemodel.h \
    control/kfttjcontrol.h \
    qchartviewer.h \
    fmginputwidget.h \
    fmgresultwidget.h \
    control/fmgcontrol.h \
    rckqinputwidget.h

RESOURCES += \
    res.qrc

INCLUDEPATH += $$PWD/chartdir60/include
LIBS += $$PWD/chartdir60/lib/chartdir60.lib
win32 {
    src_dir = $$PWD\\chartdir60\\lib\\chartdir60.dll
    CONFIG(debug, debug|release) {
        dst_dir = $$OUT_PWD\\debug\\
    } else {
        dst_dir = $$OUT_PWD\\release\\
    }
    src_dir ~= s,/,\\,g
    dst_dir ~= s,/,\\,g
    !exists($$dst_dir chartdir60.dll):system(xcopy $$src_dir $$dst_dir /Y)
}
