CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += test
DESTDIR = ../bin

QT -= gui
QT += testlib

include(../dependency.pri)

unix : LIBS += -ldl -L$$OUT_PWD/../../bin/ -lgamscpp
win32: LIBS += -L$$OUT_PWD/../../bin/ -lgamscpp

INCLUDEPATH += $$PWD/../src/
