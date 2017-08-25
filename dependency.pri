unix:GAMS_DISTRIB = $$(HOME)/gams/gams24.9_linux_x64_64_sfx
unix:GAMS_DISTRIB_API = $$(HOME)/gams/gams24.9_linux_x64_64_sfx/apifiles/C/api
win32:GAMS_DISTRIB = C:/GAMS/win64/24.9
win32:GAMS_DISTRIB_API = $$GAMS_DISTRIB/apifiles/C/api

exists($$PWD/gamsinclude.pri) {
    include($$PWD/gamsinclude.pri)
}

INCLUDEPATH += $$GAMS_DISTRIB_API

DEFINES += _CRT_SECURE_NO_WARNINGS

HEADERS = \
    $$GAMS_DISTRIB_API/gclgms.h \
    $$GAMS_DISTRIB_API/gmdcc.h \
    $$GAMS_DISTRIB_API/cfgmcc.h \
    $$GAMS_DISTRIB_API/gevmcc.h \
    $$GAMS_DISTRIB_API/gmomcc.h \
    $$GAMS_DISTRIB_API/optcc.h

SOURCES = \
    $$GAMS_DISTRIB_API/gclgms.c \
    $$GAMS_DISTRIB_API/gmdcc.c \
    $$GAMS_DISTRIB_API/cfgmcc.c \
    $$GAMS_DISTRIB_API/gevmcc.c \
    $$GAMS_DISTRIB_API/gmomcc.c \
    $$GAMS_DISTRIB_API/optcc.c

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
#DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
