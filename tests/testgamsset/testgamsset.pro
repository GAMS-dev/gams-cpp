TEMPLATE = app

DESTDIR = ../bin

include(../../dependency.pri)
include(../tests.pri)

HEADERS += \
    ../testgamsobject.h \
    testgamsset.h

SOURCES += \
    ../testgamsobject.cpp \
    testgamsset.cpp
