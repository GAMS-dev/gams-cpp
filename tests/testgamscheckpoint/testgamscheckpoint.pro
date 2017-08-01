TEMPLATE = app

DESTDIR = ../bin

include(../../dependency.pri)
include(../tests.pri)

HEADERS += \
    ../testgamsobject.h \
    testgamscheckpoint.h

SOURCES += \
    ../testgamsobject.cpp \
    testgamscheckpoint.cpp
