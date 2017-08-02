TEMPLATE = app

DESTDIR = ../bin

include(../../dependency.pri)
include(../tests.pri)

HEADERS += \
    ../testgamsobject.h \
    testgamssymbol.h

SOURCES += \
    ../testgamsobject.cpp \
    testgamssymbol.cpp
