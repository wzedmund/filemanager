#-------------------------------------------------
#
# Project created by QtCreator 2016-02-01T15:46:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = filemanager
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    treefilterproxymodel.cpp \
    viewfilterproxymodel.cpp \
    model.cpp \
    modelitem.cpp \
    modelcompleter.cpp \
    searchbar.cpp \
    tabbar.cpp \
    addresstoolbar.cpp \
    tabbaritem.cpp

HEADERS  += mainwindow.h \
    treefilterproxymodel.h \
    viewfilterproxymodel.h \
    model.h \
    modelitem.h \
    modelcompleter.h \
    searchbar.h \
    tabbar.h \
    addresstoolbar.h \
    tabbaritem.h

FORMS    +=
