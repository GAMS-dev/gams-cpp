TEMPLATE = app

DESTDIR = ../bin
TEST = $$PWD/..
DEP = $$TEST/../dep

include(../../dependency.pri)
include(../tests.pri)

HEADERS += \
    ../testgamsobject.h \
    testgamsworkspaceinfo.h

SOURCES += \
    ../testgamsobject.cpp \
    testgamsworkspaceinfo.cpp
