#-------------------------------------------------
#
# Project created by QtCreator 2015-09-04T15:40:45
#
#-------------------------------------------------

QT       += testlib

TARGET = tst_qimagegridtest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_qimagegridtest.cpp \
    qimagegrid.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    qimagegrid.hpp

QMAKE_CXXFLAGS += -std=c++14

RESOURCES += \
    resources.qrc
