TEMPLATE = app

DESTDIR = ../bin

include(../../dependency.pri)
include(../tests.pri)

HEADERS += \
    ../testgamsobject.h \
    testgamsmodelinstanceopt.h

SOURCES += \
    ../testgamsobject.cpp \
    testgamsmodelinstanceopt.cpp
