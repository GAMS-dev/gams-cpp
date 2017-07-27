CONFIG += console c++11
CONFIG -= app_bundle
DESTDIR = ../../bin

unix : LIBS += -ldl -L../../bin -lgamscpp
win32: LIBS += -L../../bin/ -lgamscpp0

INCLUDEPATH +=  ../../src/     \
                ../transport
