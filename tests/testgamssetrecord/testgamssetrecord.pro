TEMPLATE = app

DESTDIR = ../bin

include(../../dependency.pri)
include(../tests.pri)

HEADERS += \
    ../testgamsobject.h \
    testgamssetrecord.h

SOURCES += \
    ../testgamsobject.cpp \
    testgamssetrecord.cpp
