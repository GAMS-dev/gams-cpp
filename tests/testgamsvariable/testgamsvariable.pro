TEMPLATE = app

DESTDIR = ../bin
TEST = $$PWD/..
DEP = $$TEST/../dep

include(../../dependency.pri)
include(../tests.pri)

HEADERS += \
    ../testgamsobject.h \
    testgamsvariable.h

SOURCES += \
    ../testgamsobject.cpp \
    testgamsvariable.cpp
