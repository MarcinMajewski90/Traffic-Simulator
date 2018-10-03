#-------------------------------------------------
#
# Project created by QtCreator 2018-03-01T13:08:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = symbymouse
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mapa.cpp \
    car.cpp \
    tablicamapy.cpp

HEADERS  += mainwindow.h \
    mapa.h \
    car.h \
    tablicamapy.h

FORMS    += mainwindow.ui

RESOURCES += \
    img.qrc
