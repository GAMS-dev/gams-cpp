TEMPLATE = app

DESTDIR = ../bin

include(../../dependency.pri)
include(../tests.pri)

HEADERS += \
    ../testgamsobject.h \
    testgamsdatabase.h

SOURCES += \
    ../testgamsobject.cpp \
    testgamsdatabase.cpp
