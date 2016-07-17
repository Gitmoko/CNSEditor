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
    csubs.cpp \
    statetreeitem.cpp

HEADERS  += mainwindow.h \
    child.h \
    states.h \
    addstatesdialog.h \
    cns.h \
    csubs.h \
    statetreeitem.h

FORMS    += mainwindow.ui \
    child.ui \
    addstatesdialog.ui \
    csubs.ui




win32: LIBS += -L$$PWD/../../../../Mylibs/MyParser/Debug/x64/ -lDLLTest

INCLUDEPATH += $$PWD/../../../../Mylibs/MyParser/include/ \
		$$PWD/../../../../Mylibs/boost_1_60_0
DEPENDPATH += $$PWD/../../../../Mylibs/MyParser/include/

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../../../../Mylibs/MyParser/Debug/x64/DLLTest.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../../../../Mylibs/MyParser/Debug/x64/libDLLTest.a
