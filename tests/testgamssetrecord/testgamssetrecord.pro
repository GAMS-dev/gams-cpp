TEMPLATE = app

DESTDIR = ../bin
TEST = $$PWD/..
DEP = $$TEST/../dep

include(../../dependency.pri)
include(../tests.pri)

HEADERS += \
    ../testgamsobject.h \
    testgamssetrecord.h

SOURCES += \
    ../testgamsobject.cpp \
    testgamssetrecord.cpp
