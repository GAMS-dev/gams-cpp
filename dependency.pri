unix:GAMS_DISTRIB = $$(HOME)/gams/gams24.8_linux_x64_64_sfx/apifiles/C/api
win32:GAMS_DISTRIB = D:\home\jmonki\vs8_alpha\src\apiexamples\C\api

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
