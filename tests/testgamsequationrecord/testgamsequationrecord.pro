TEMPLATE = app

DESTDIR = ../bin

include(../../dependency.pri)
include(../tests.pri)

HEADERS += \
    ../testgamsobject.h \
    testgamsequationrecord.h

SOURCES += \
    ../testgamsobject.cpp \
    testgamsequationrecord.cpp
