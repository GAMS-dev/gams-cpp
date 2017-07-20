TEMPLATE = app

DESTDIR = ../bin
TEST = $$PWD/..
DEP = $$TEST/../dep

include(../../dependency.pri)
include(../tests.pri)

HEADERS += \
    ../testgamsobject.h \
    testgamsparameterrecord.h

SOURCES += \
    ../testgamsobject.cpp \
    testgamsparameterrecord.cpp
