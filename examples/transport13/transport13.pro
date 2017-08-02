TEMPLATE = app

include(../../dependency.pri)
include(../examples.pri)

INCLUDEPATH += ../transport

SOURCES += \
    transport13.cpp \
    ../transport/transport.cpp
