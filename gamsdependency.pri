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

win32 {
    # Switch off warings caused by GAMS headers
    DEFINES += _CRT_SECURE_NO_WARNINGS
}

include (version)
# GAMS_CORE_PATH is Jenkins build switch
GAMS_CORE_TMP = $$(GAMS_CORE_PATH)
!exists($$PWD/gamsinclude.pri) {
    equals(GAMS_CORE_TMP, "") {
        macx {
            GAMSINC = GAMS_DISTRIB=/Applications/GAMS'$$GAMS_DISTRIB_MAJOR'.'$$GAMS_DISTRIB_MINOR'/sysdir \
                      GAMS_DISTRIB_C_API=\$$GAMS_DISTRIB/apifiles/C/api   \
                      GAMS_DISTRIB_CPP_API=\$$GAMS_DISTRIB/apifiles/C++/api
        }
        unix:!macx {
            GAMSINC = GAMS_DISTRIB=$$(HOME)/gams/gams'$$GAMS_DISTRIB_MAJOR'.'$$GAMS_DISTRIB_MINOR'_linux_x64_64_sfx \
                      GAMS_DISTRIB_C_API=\$$GAMS_DISTRIB/apifiles/C/api   \
                      GAMS_DISTRIB_CPP_API=\$$GAMS_DISTRIB/apifiles/C++/api
        }
        win32 {
            GAMSINC = GAMS_DISTRIB=C:/GAMS/win64/'$$GAMS_DISTRIB_MAJOR'.'$$GAMS_DISTRIB_MINOR' \
                      GAMS_DISTRIB_C_API=\$$GAMS_DISTRIB/apifiles/C/api   \
                      GAMS_DISTRIB_CPP_API=\$$GAMS_DISTRIB/apifiles/C++/api
        }
        write_file($$PWD/gamsinclude.pri,GAMSINC)
    } else {
        GAMSINC = GAMS_DISTRIB=$$(GAMS_CORE_PATH) \
                  GAMS_DISTRIB_C_API=\$$GAMS_DISTRIB/apifiles/C/api   \
                  GAMS_DISTRIB_CPP_API=\$$GAMS_DISTRIB/apifiles/C++/api
        write_file($$PWD/gamsinclude.pri,GAMSINC)
    }
}

exists($$PWD/gamsinclude.pri) {
    include($$PWD/gamsinclude.pri)
}

# GAMS_BUILD is GAMS distrib build switch
GAMS_BUILD_ENV = $$(GAMS_BUILD)
equals(GAMS_BUILD_ENV, "") {
    INCLUDEPATH += $$GAMS_DISTRIB_C_API     \
                   $$GAMS_DISTRIB_CPP_API

    SOURCES += \
        $$GAMS_DISTRIB_C_API/gclgms.c               \
        $$GAMS_DISTRIB_C_API/gmdcc.c                \
        $$GAMS_DISTRIB_C_API/cfgmcc.c               \
        $$GAMS_DISTRIB_C_API/gevmcc.c               \
        $$GAMS_DISTRIB_C_API/gmomcc.c               \
        $$GAMS_DISTRIB_C_API/optcc.c                \
        $$GAMS_DISTRIB_CPP_API/gamsoptions.cpp      \
        $$GAMS_DISTRIB_CPP_API/gamsoptionsimpl.cpp
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
    INCLUDEPATH += \
        $$(GPRODUCTS)/gclib                     \
        $$(GPRODUCTS)/apiwrap/gmdobj            \
        $$(GPRODUCTS)/apiwrap/joat              \
        $$(GPRODUCTS)/apiwrap/optobj            \
        $$(GPRODUCTS)/src/apiexamples/C++/api

    SOURCES += \
        $$(GPRODUCTS)/gclib/gclgms.c            \
        $$(GPRODUCTS)/gclib/gcmt.c              \
        $$(GPRODUCTS)/apiwrap/gmdobj/gmdcc.c    \
        $$(GPRODUCTS)/apiwrap/joat/cfgmcc.c     \
        $$(GPRODUCTS)/apiwrap/joat/gevmcc.c     \
        $$(GPRODUCTS)/apiwrap/joat/gmomcc.c     \
        $$(GPRODUCTS)/apiwrap/optobj/optcc.c    \
        $$(GPRODUCTS)/gamsoptions.cpp           \
        $$(GPRODUCTS)/gamsoptionsimpl.cpp
}
