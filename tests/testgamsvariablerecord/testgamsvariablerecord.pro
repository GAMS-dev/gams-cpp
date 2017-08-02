TEMPLATE = app

DESTDIR = ../bin

include(../../dependency.pri)
include(../tests.pri)

HEADERS += \
    ../testgamsobject.h \
    testgamsvariablerecord.h

SOURCES += \
    ../testgamsobject.cpp \
    testgamsvariablerecord.cpp
