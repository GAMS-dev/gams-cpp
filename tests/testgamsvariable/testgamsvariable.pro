TEMPLATE = app

DESTDIR = ../bin

include(../../dependency.pri)
include(../tests.pri)

HEADERS += \
    ../testgamsobject.h \
    testgamsvariable.h

SOURCES += \
    ../testgamsobject.cpp \
    testgamsvariable.cpp
