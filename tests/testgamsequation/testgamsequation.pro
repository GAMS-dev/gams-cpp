TEMPLATE = app

DESTDIR = ../bin

include(../../dependency.pri)
include(../tests.pri)

HEADERS += \
    ../testgamsobject.h \
    testgamsequation.h

SOURCES += \
    ../testgamsobject.cpp \
    testgamsequation.cpp
