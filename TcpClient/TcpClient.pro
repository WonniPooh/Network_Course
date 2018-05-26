#-------------------------------------------------
#
# Project created by QtCreator 2016-11-12T03:24:10
#
#-------------------------------------------------

QT       += network
QT       -= gui

QMAKE_CXXFLAGS += -std=c++11
CONFIG += staticlib

TARGET = TcpClient
TEMPLATE = lib
CONFIG += staticlib

SOURCES += tcpclient.cpp \
    tcpthreadpart.cpp

HEADERS += tcpclient.h \
    tcpthreadpart.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
