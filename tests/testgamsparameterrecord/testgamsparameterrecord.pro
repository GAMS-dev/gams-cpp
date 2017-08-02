TEMPLATE = app

DESTDIR = ../bin

include(../../dependency.pri)
include(../tests.pri)

HEADERS += \
    ../testgamsobject.h \
    testgamsparameterrecord.h

SOURCES += \
    ../testgamsobject.cpp \
    testgamsparameterrecord.cpp
