TEMPLATE = app

DESTDIR = ../bin
TEST = $$PWD/..
DEP = $$TEST/../dep

include(../../dependency.pri)
include(../tests.pri)

HEADERS += \
    ../testgamsobject.h \
    testgamsworkspace.h

SOURCES += \
    ../testgamsobject.cpp \
    testgamsworkspace.cpp
