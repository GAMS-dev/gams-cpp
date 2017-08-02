TEMPLATE = app

DESTDIR = ../bin

include(../../dependency.pri)
include(../tests.pri)

HEADERS += \
    ../testgamsobject.h \
    testgamsparameter.h

SOURCES += \
    ../testgamsobject.cpp \
    testgamsparameter.cpp
