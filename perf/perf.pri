CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += test
DESTDIR = ../bin

QT -= gui
QT += testlib

include(../dependency.pri)

unix : LIBS += -ldl -L$$DESTDIR -lgamscpp
win32: LIBS += -L$$DESTDIR/ -lgamscpp

INCLUDEPATH += $$PWD/../src/
