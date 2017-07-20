TEMPLATE = app

DESTDIR = ../bin
TEST = $$PWD/..
DEP = $$TEST/../dep

include(../../dependency.pri)
include(../tests.pri)

HEADERS += \
    ../testgamsobject.h \
    testgamsdomain.h

SOURCES += \
    ../testgamsobject.cpp \
    testgamsdomain.cpp
