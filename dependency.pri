
# GAMS_CORE_PATH is Jenkins build switch
GAMS_CORE_TMP = $$(GAMS_CORE_PATH)
!exists($$PWD/gamsinclude.pri) {
    equals(GAMS_CORE_TMP, "") {
        macx {
            GAMSINC = GAMS_DISTRIB=/Applications/GAMS25.1/sysdir \
                      GAMS_DISTRIB_API=\$$GAMS_DISTRIB/apifiles/C/api
        }
        unix:!macx {
            GAMSINC = GAMS_DISTRIB=$$(HOME)/gams/gams25.1_linux_x64_64_sfx \
                      GAMS_DISTRIB_API=\$$GAMS_DISTRIB/apifiles/C/api
        }
        win32 {
            GAMSINC = GAMS_DISTRIB=C:/GAMS/win64/25.1 \
                      GAMS_DISTRIB_API=\$$GAMS_DISTRIB/apifiles/C/api
        }
        write_file($$PWD/gamsinclude.pri,GAMSINC)
    } else {
        GAMSINC = GAMS_DISTRIB=$$(GAMS_CORE_PATH) \
                  GAMS_DISTRIB_API=\$$GAMS_DISTRIB/apifiles/C/api
        write_file($$PWD/gamsinclude.pri,GAMSINC)
    }
}

exists($$PWD/gamsinclude.pri) {
    include($$PWD/gamsinclude.pri)
}

# GAMS_BULD is GAMS distrib build switch
GAMS_BUILD_ENV = $$(GAMS_BUILD)
equals(GAMS_BUILD_ENV, "") {
    INCLUDEPATH += $$GAMS_DISTRIB_API

    SOURCES = \
        $$GAMS_DISTRIB_API/gclgms.c \
        $$GAMS_DISTRIB_API/gmdcc.c  \
        $$GAMS_DISTRIB_API/cfgmcc.c \
        $$GAMS_DISTRIB_API/gevmcc.c \
        $$GAMS_DISTRIB_API/gmomcc.c \
        $$GAMS_DISTRIB_API/optcc.c
} else {
    DEFINES += HAVE_MUTEX
    GSYS_ENV = $$(GSYS)
    equals(GSYS_ENV, "wei") {
        DEFINES += WEI 
        DEFINES += CIA_WEX
    }
    equals(GSYS_ENV, "vs8") {
        DEFINES += VS8 
        DEFINES += CIA_WIN
    }
    equals(GSYS_ENV, "leg") {
        DEFINES += LEG 
        DEFINES += CIA_LEX
        QMAKE_CXXFLAGS += -Werror-implicit-function-declaration -Wreturn-type -Wmissing-declarations -m64
        QMAKE_CFLAGS += -Werror-implicit-function-declaration -Wreturn-type -Wmissing-prototypes -Wmissing-declarations -m64
    }
    equals(GSYS_ENV, "deg") {
        DEFINES += DEG 
        DEFINES += CIA_DEX
        QMAKE_CXXFLAGS += -Werror-implicit-function-declaration -Wreturn-type -Wmissing-declarations -m64
        QMAKE_CFLAGS += -Werror-implicit-function-declaration -Wreturn-type -Wmissing-prototypes -Wmissing-declarations -m64
    }
    INCLUDEPATH += $$(GPRODUCTS)/gclib $$(GPRODUCTS)/apiwrap/gmdobj $$(GPRODUCTS)/apiwrap/joat $$(GPRODUCTS)/apiwrap/optobj

    SOURCES = \
        $$(GPRODUCTS)/gclib/gclgms.c \
        $$(GPRODUCTS)/gclib/gcmt.c \
        $$(GPRODUCTS)/apiwrap/gmdobj/gmdcc.c  \
        $$(GPRODUCTS)/apiwrap/joat/cfgmcc.c \
        $$(GPRODUCTS)/apiwrap/joat/gevmcc.c \
        $$(GPRODUCTS)/apiwrap/joat/gmomcc.c \
        $$(GPRODUCTS)/apiwrap/optobj/optcc.c
}

DEFINES += _CRT_SECURE_NO_WARNINGS

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
