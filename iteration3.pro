#-------------------------------------------------
#
# Project created by QtCreator 2016-03-08T19:02:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DashWithTemp
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qgauge.cpp \
    interfacecan.cpp \
    can/canrecieve.cpp \
    can/cansend.cpp

HEADERS  += mainwindow.h \
    qgauge.h \
    interfacecan.h \
    can/can.h \
    can/cancomm.h \
    can/canframelen.h \
    can/error.h \
    can/gw.h \
    can/isotp.h \
    can/lib.h \
    can/netlink.h \
    can/raw.h \
    can/terminal.h

FORMS    += mainwindow.ui
