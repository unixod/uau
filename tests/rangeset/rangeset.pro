#-------------------------------------------------
#
# Project created by QtCreator 2012-07-08T14:18:13
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_rangeset
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_rangeset.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH += ../../uau
QMAKE_CXXFLAGS += -std=c++0x

