TEMPLATE = app

DESTDIR = ../bin

include(../../dependency.pri)
include(../tests.pri)

HEADERS += \
    ../testgamsobject.h \
    testgamssymboliter.h

SOURCES += \
    ../testgamsobject.cpp \
    testgamssymboliter.cpp
