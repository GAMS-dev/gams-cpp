TEMPLATE = app

DESTDIR = ../bin

include(../../dependency.pri)
include(../tests.pri)

HEADERS += \
    ../testgamsobject.h \
    testgamsdomain.h

SOURCES += \
    ../testgamsobject.cpp \
    testgamsdomain.cpp
