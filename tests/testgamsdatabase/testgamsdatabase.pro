TEMPLATE = app

DESTDIR = ../bin
TEST = $$PWD/..
DEP = $$TEST/../dep

include(../../dependency.pri)
include(../tests.pri)

HEADERS += \
    ../testgamsobject.h \
    testgamsdatabase.h

SOURCES += \
    ../testgamsobject.cpp \
    testgamsdatabase.cpp
