TEMPLATE = app

DESTDIR = ../bin
TEST = $$PWD/..
DEP = $$TEST/../dep

include(../../dependency.pri)
include(../tests.pri)

HEADERS += \
    ../testgamsobject.h \
    testgamsequation.h

SOURCES += \
    ../testgamsobject.cpp \
    testgamsequation.cpp
