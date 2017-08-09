TEMPLATE = app

include(../examples.pri)

INCLUDEPATH += ../transport

SOURCES += \
    transport13.cpp \
    ../transport/transport.cpp
