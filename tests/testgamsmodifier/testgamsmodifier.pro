TEMPLATE = app

DESTDIR = ../bin

include(../../dependency.pri)
include(../tests.pri)

HEADERS += \
    ../testgamsobject.h \
    testgamsmodifier.h

SOURCES += \
    ../testgamsobject.cpp \
    testgamsmodifier.cpp
