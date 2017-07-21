TEMPLATE = app

DESTDIR = ../bin
TEST = $$PWD/..
DEP = $$TEST/../dep

include(../../dependency.pri)
include(../tests.pri)

HEADERS += \
    ../testgamsobject.h \
    testgamsexception.h

SOURCES += \
    ../testgamsobject.cpp \
    testgamsexception.cpp