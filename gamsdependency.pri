#
# GAMS - General Algebraic Modeling System C++ API
#
# Copyright (c) 2017-2018 GAMS Software GmbH <support@gams.com>
# Copyright (c) 2017-2018 GAMS Development Corp. <support@gams.com>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#

include (version)
# GAMS_CORE_PATH is Jenkins build switch
GAMS_CORE_TMP = $$(GAMS_CORE_PATH)
!exists($$PWD/gamsinclude.pri) {
    equals(GAMS_CORE_TMP, "") {
        macx {
            GAMSINC = GAMS_DISTRIB=/Applications/GAMS'$$GAMS_DISTRIB_MAJOR'.'$$GAMS_DISTRIB_MINOR'/sysdir \
                      GAMS_DISTRIB_API=\$$GAMS_DISTRIB/apifiles/C/api
        }
        unix:!macx {
            GAMSINC = GAMS_DISTRIB=$$(HOME)/gams/gams'$$GAMS_DISTRIB_MAJOR'.'$$GAMS_DISTRIB_MINOR'_linux_x64_64_sfx \
                      GAMS_DISTRIB_API=\$$GAMS_DISTRIB/apifiles/C/api
        }
        win32 {
            GAMSINC = GAMS_DISTRIB=C:/GAMS/win64/'$$GAMS_DISTRIB_MAJOR'.'$$GAMS_DISTRIB_MINOR' \
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

# GAMS_BUILD is GAMS distrib build switch
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
