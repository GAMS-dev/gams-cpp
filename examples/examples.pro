# ---------------------------- Versions ------------------------------
VERSION = 0.1.1
GAMS_MIN_VER = 24.8.0
CLIB_MIN_VER = 3.1.0
# --------------------------------------------------------------------

TEMPLATE = app
DESTDIR = ../bin

QT += core
QT -= gui

CONFIG += c++11

TARGET = examples
CONFIG += console
CONFIG -= app_bundle

include(../dependency.pri)

unix : LIBS += -ldl -L../bin -lgamscpp
win32: LIBS += -L../bin/ -lgamscpp0

INCLUDEPATH +=  ../src/     \
                Transport

SOURCES += main.cpp \
    Transport/transport.cpp \
    Transport1/transport1.cpp \
    Transport10/transport10.cpp \
    Transport11/transport11.cpp \
    Transport12/transport12.cpp \
    Transport13/transport13.cpp \
    Transport14/transport14.cpp \
    Transport2/transport2.cpp \
    Transport3/transport3.cpp \
    Transport4/transport4.cpp \
    Transport5/transport5.cpp \
    Transport6/transport6.cpp \
    Transport7/transport7.cpp \
    Transport8/transport8.cpp \
    Transport9/transport9.cpp \
    Warehouse/warehouse.cpp \
    DomainChecking/domainchecking.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    Transport/transport.h \
    Transport1/transport1.h \
    Transport10/transport10.h \
    Transport11/transport11.h \
    Transport12/transport12.h \
    Transport13/transport13.h \
    Transport14/transport14.h \
    Transport2/transport2.h \
    Transport3/transport3.h \
    Transport4/transport4.h \
    Transport5/transport5.h \
    Transport6/transport6.h \
    Transport7/transport7.h \
    Transport8/transport8.h \
    Transport9/transport9.h \
    Warehouse/warehouse.h \
    DomainChecking/domainchecking.h
