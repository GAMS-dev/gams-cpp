TEMPLATE = app

DESTDIR = ../bin
TEST = $$PWD/..
DEP = $$TEST/../dep

include(../../dependency.pri)
include(../tests.pri)

HEADERS += \
    ../testgamsobject.h \
    testgamssymbol.h

SOURCES += \
    ../testgamsobject.cpp \
    testgamssymbol.cpp
