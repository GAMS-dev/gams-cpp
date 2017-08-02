TEMPLATE = app

DESTDIR = ../bin

include(../../dependency.pri)
include(../tests.pri)

HEADERS += \
    ../testgamsobject.h \
    testgamssymbolrecord.h

SOURCES += \
    ../testgamsobject.cpp \
    testgamssymbolrecord.cpp
