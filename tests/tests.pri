CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += test
DESTDIR = ../bin

QT += testlib
QT -= gui

unix : LIBS += -ldl -L../../bin -lgamscpp
win32: LIBS += -L../../bin/ -lgamscpp0

INCLUDEPATH += $$TEST $$TEST/../src/
