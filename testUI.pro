#-------------------------------------------------
#
# Project created by QtCreator 2016-06-02T13:17:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = testUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    child.cpp \
    states.cpp \
    addstatesdialog.cpp \
    cns.cpp \
    csubs.cpp

HEADERS  += mainwindow.h \
    child.h \
    states.h \
    addstatesdialog.h \
    cns.h \
    csubs.h

FORMS    += mainwindow.ui \
    child.ui \
    addstatesdialog.ui \
    csubs.ui
