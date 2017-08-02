TEMPLATE = app

DESTDIR = ../bin

include(../../dependency.pri)
include(../tests.pri)

HEADERS += \
    ../testgamsobject.h \
    testgamsjob.h

SOURCES += \
    ../testgamsobject.cpp \
    testgamsjob.cpp
