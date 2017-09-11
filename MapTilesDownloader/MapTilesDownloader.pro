#-------------------------------------------------
#
# Project created by QtCreator 2016-04-08T11:00:38
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MapDownloader
TEMPLATE = app


SOURCES += main.cpp\
        View/mainview.cpp \
    ViewModel/mainviewmanager.cpp \
    startup.cpp \
    Model/tile.cpp \
    Model/mapdownloader.cpp

HEADERS  += View/mainview.h \
    ViewModel/mainviewmanager.h \
    startup.h \
    Model/tile.h \
    Model/mapdownloader.h

FORMS    += View/mainview.ui


RESOURCES += \
    resources.qrc
