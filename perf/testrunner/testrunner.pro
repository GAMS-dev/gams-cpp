TEMPLATE = app

DESTDIR = ../bin

include(../perf.pri)

HEADERS += \
    testperformance.h \
    testrunner.h

SOURCES += \
    testperformance.cpp \
    testrunner.cpp
