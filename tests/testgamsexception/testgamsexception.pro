TEMPLATE = app

DESTDIR = ../bin

include(../../dependency.pri)
include(../tests.pri)

HEADERS += \
    ../testgamsobject.h \
    testgamsexception.h

SOURCES += \
    ../testgamsobject.cpp \
    testgamsexception.cpp
