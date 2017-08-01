TEMPLATE = app

DESTDIR = ../bin

include(../../dependency.pri)
include(../tests.pri)

HEADERS += \
    ../testgamsobject.h \
    testgamsworkspace.h

SOURCES += \
    ../testgamsobject.cpp \
    testgamsworkspace.cpp
