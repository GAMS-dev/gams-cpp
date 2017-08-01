TEMPLATE = app

include(../../dependency.pri)
include(../examples.pri)

INCLUDEPATH += ../Transport

SOURCES += \
    transport13.cpp \
    ../Transport/transport.cpp
