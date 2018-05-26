QT      += core gui
QT      += network
QT      += widgets

CONFIG += c++11

TARGET = oly_server
CONFIG += console

TEMPLATE = app

SOURCES += main.cpp \
    olyserver.cpp \
    clientconnection.cpp \
    clientnames.cpp \
    olyserverwidget.cpp

HEADERS += \
    olyserver.h \
    clientconnection.h \
    clientnames.h \
    olyserverwidget.h
