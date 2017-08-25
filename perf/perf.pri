CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += test
DESTDIR = ../bin

QT -= gui
QT += testlib

include(../dependency.pri)

unix : LIBS += -ldl -L$$GAMS_DISTRIB/ -lgamscpp
win32: LIBS += -L$$GAMS_DISTRIB/ -lgamscpp

INCLUDEPATH += $$PWD/../src/
