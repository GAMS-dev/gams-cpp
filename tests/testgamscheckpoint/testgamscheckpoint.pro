TEMPLATE = app

DESTDIR = ../bin
TEST = $$PWD/..
DEP = $$TEST/../dep

include(../../dependency.pri)
include(../tests.pri)

HEADERS += \
    ../testgamsobject.h \
    testgamscheckpoint.h

SOURCES += \
    ../testgamsobject.cpp \
    testgamscheckpoint.cpp
