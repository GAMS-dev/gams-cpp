TEMPLATE = app

DESTDIR = ../bin

include(../../dependency.pri)
include(../tests.pri)

HEADERS += \
    ../testgamsobject.h \
    testgamsmodelinstance.h

SOURCES += \
    ../testgamsobject.cpp \
    testgamsmodelinstance.cpp
