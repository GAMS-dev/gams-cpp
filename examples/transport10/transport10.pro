TEMPLATE = app

include(../../dependency.pri)
include(../examples.pri)

win32:QT += axcontainer

SOURCES += transport10.cpp
