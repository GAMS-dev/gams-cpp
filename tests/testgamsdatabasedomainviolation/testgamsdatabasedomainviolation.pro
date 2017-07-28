TEMPLATE = app

DESTDIR = ../bin

include(../../dependency.pri)
include(../tests.pri)

HEADERS += \
    ../testgamsobject.h \
    testgamsdatabasedomainviolation.h

SOURCES += \
    ../testgamsobject.cpp \
    testgamsdatabasedomainviolation.cpp
