TEMPLATE = app

DESTDIR = ../bin

include(../../dependency.pri)
include(../tests.pri)

HEADERS += \
    ../testgamsobject.h \
    testgamssymboldomainviolation.h

SOURCES += \
    ../testgamsobject.cpp \
    testgamssymboldomainviolation.cpp
