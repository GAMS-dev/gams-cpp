TEMPLATE = app

DESTDIR = ../bin
TEST = $$PWD/..
DEP = $$TEST/../dep

include(../../dependency.pri)
include(../tests.pri)

HEADERS += \
    ../testgamsobject.h \
    testgamsdatabasedomainviolation.h

SOURCES += \
    ../testgamsobject.cpp \
    testgamsdatabasedomainviolation.cpp
