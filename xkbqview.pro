#-------------------------------------------------
#
# Project created by QtCreator 2012-11-18T16:53:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = xkbqview
TEMPLATE = app

LIBS += -lX11 -lxkbfile

SOURCES += main.cpp\
        mainwindow.cpp \
    xkbviewer.cpp \
    xkbinfo.cpp \
    xkbgeom.cpp

HEADERS  += mainwindow.h \
    xkbviewer.h \
    xkbinfo.h \
    xkbgeom.h

FORMS    += mainwindow.ui
