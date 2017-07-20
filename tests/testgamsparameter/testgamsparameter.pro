TEMPLATE = app

DESTDIR = ../bin
TEST = $$PWD/..
DEP = $$TEST/../dep

include(../../dependency.pri)
include(../tests.pri)

HEADERS += \
    ../testgamsobject.h \
    testgamsparameter.h

SOURCES += \
    ../testgamsobject.cpp \
    testgamsparameter.cpp
