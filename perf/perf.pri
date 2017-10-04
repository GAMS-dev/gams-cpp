CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += test
DESTDIR = ../bin

QT -= gui
QT += testlib

include(../dependency.pri)

message($$OUT_PWD)
message($$PWD)
unix : LIBS += -ldl -L$$OUT_PWD/../../bin/ -lgamscpp
win32: LIBS += -L$$OUT_PWD/../../bin/ -lgamscpp

INCLUDEPATH += $$PWD/../src/
