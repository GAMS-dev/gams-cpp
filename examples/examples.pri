CONFIG += console c++11
CONFIG -= app_bundle

exists($$PWD/../gams-cpp.pro) {
    DESTDIR = ../../bin
    unix : LIBS += -ldl -L../../bin -lgamscpp
    win32: LIBS += -L../../bin/ -lgamscpp
    INCLUDEPATH += ../../src/
} else {
    unix : LIBS += -ldl -L../lib -lgamscpp
    win32: LIBS += -L../lib/$$VSVERSION -lgamscpp
    INCLUDEPATH += ../api
}
