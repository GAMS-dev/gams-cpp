TEMPLATE = app

DESTDIR = ../bin
TEST = $$PWD/..
DEP = $$TEST/../dep

include(../../dependency.pri)
include(../tests.pri)

HEADERS += \
    ../testgamsobject.h \
    testgamsvariablerecord.h

SOURCES += \
    ../testgamsobject.cpp \
    testgamsvariablerecord.cpp
