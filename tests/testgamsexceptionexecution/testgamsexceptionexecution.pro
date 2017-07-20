TEMPLATE = app

DESTDIR = ../bin
TEST = $$PWD/..
DEP = $$TEST/../dep

include(../../dependency.pri)
include(../tests.pri)

HEADERS += \
    ../testgamsobject.h \
    testgamsexceptionexecution.h

SOURCES += \
    ../testgamsobject.cpp \
    testgamsexceptionexecution.cpp
