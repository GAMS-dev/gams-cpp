TEMPLATE = app

DESTDIR = ../bin

include(../../dependency.pri)
include(../tests.pri)

HEADERS += \
    ../testgamsobject.h \
    testgamsworkspaceinfo.h

SOURCES += \
    ../testgamsobject.cpp \
    testgamsworkspaceinfo.cpp
