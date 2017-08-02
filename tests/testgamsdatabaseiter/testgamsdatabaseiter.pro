TEMPLATE = app

DESTDIR = ../bin

include(../../dependency.pri)
include(../tests.pri)

HEADERS += \
    ../testgamsobject.h \
    testgamsdatabaseiter.h

SOURCES += \
    ../testgamsobject.cpp \
    testgamsdatabaseiter.cpp
