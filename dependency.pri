#---------------------------- Versions ------------------------------
VERSION = 0.1.1
GAMS_MIN_VER = 24.8.0
CLIB_MIN_VER = 3.1.0
#--------------------------------------------------------------------

unix:GAMS_DISTRIB = $$(HOME)/gams/gams24.9_linux_x64_64_sfx/apifiles/C/api
win32:GAMS_DISTRIB = C:\GAMS\win64\24.9\apifiles\C\api

INCLUDEPATH += $$GAMS_DISTRIB

DEFINES += _CRT_SECURE_NO_WARNINGS

HEADERS = \
    $$GAMS_DISTRIB/gclgms.h \
    $$GAMS_DISTRIB/gmdcc.h \
    $$GAMS_DISTRIB/cfgmcc.h \
    $$GAMS_DISTRIB/gevmcc.h \
    $$GAMS_DISTRIB/gmomcc.h \
    $$GAMS_DISTRIB/optcc.h

SOURCES = \
    $$GAMS_DISTRIB/gclgms.c \
    $$GAMS_DISTRIB/gmdcc.c \
    $$GAMS_DISTRIB/cfgmcc.c \
    $$GAMS_DISTRIB/gevmcc.c \
    $$GAMS_DISTRIB/gmomcc.c \
    $$GAMS_DISTRIB/optcc.c

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
#DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
