QT += core
QT += network
QT += gui
QT += widgets

CONFIG += c++11

TARGET = Project
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    requestsender.cpp \
    replyhandler.cpp \
    mainwindow.cpp

HEADERS += \
    requestsender.h \
    replyhandler.h \
    mainwindow.h

INCLUDEPATH += $$PWD/../TcpClient

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-TcpClient-Desktop-Release/release/ -lTcpClient
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-TcpClient-Desktop-Release/debug/ -lTcpClient
else:unix: LIBS += -L$$PWD/../build-TcpClient-Desktop-Release/ -lTcpClient

INCLUDEPATH += $$PWD/../TcpClient
DEPENDPATH += $$PWD/../TcpClient

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../build-TcpClient-Desktop-Release/release/libTcpClient.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../build-TcpClient-Desktop-Release/debug/libTcpClient.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../build-TcpClient-Desktop-Release/release/TcpClient.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../build-TcpClient-Desktop-Release/debug/TcpClient.lib
else:unix: PRE_TARGETDEPS += $$PWD/../build-TcpClient-Desktop-Release/libTcpClient.a
