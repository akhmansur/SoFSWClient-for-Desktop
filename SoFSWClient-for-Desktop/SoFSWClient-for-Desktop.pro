#-------------------------------------------------
#
# Project created by QtCreator 2016-02-24T17:29:02
#
#-------------------------------------------------

QT       += core gui xml network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SoFSWClient-for-Desktop
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    networkworker.cpp

HEADERS  += mainwindow.h \
    networkworker.h

FORMS    += mainwindow.ui

RESOURCES += \
    images.qrc
