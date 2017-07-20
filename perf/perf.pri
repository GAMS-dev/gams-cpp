CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += test
DESTDIR = ../bin

QT -= gui
QT += testlib

TEST = $$PWD/..
DEP = $$TEST/dep

include(../dependency.pri)

unix : LIBS += -ldl -L../../bin -lgamscpp
win32: LIBS += -L../../bin/ -lgamscpp0

INCLUDEPATH += $$PWD/../src/
