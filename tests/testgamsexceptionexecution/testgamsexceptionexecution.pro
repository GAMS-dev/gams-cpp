TEMPLATE = app

DESTDIR = ../bin

include(../../dependency.pri)
include(../tests.pri)

HEADERS += \
    ../testgamsobject.h \
    testgamsexceptionexecution.h

SOURCES += \
    ../testgamsobject.cpp \
    testgamsexceptionexecution.cpp
