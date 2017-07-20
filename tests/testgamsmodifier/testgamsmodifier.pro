TEMPLATE = app

DESTDIR = ../bin
TEST = $$PWD/..
DEP = $$TEST/../dep

include(../../dependency.pri)
include(../tests.pri)

HEADERS += \
    ../testgamsobject.h \
    testgamsmodifier.h

SOURCES += \
    ../testgamsobject.cpp \
    testgamsmodifier.cpp
