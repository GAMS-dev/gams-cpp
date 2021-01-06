# Generated from testgamsplatform.pro.

#####################################################################
## testgamsplatform Binary:
#####################################################################

qt_add_executable(testgamsplatform
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../bin"
    SOURCES
        ../testgamsobject.cpp ../testgamsobject.h
        testgamsplatform.cpp testgamsplatform.h
    DEFINES
        API_VERSION=\\\"'0'.'11'.'0'\\\"
    INCLUDE_DIRECTORIES
        ..
        ../../src
        ../../src/generated
    PUBLIC_LIBRARIES
        Qt::Test
)

#### Keys ignored in scope 1:.:.:testgamsplatform.pro:<TRUE>:
# TEMPLATE = "app"

## Scopes:
#####################################################################

qt_extend_target(testgamsplatform CONDITION WIN32
    SOURCES
        ../../src/gamspath.cpp
    DEFINES
        _CRT_SECURE_NO_WARNINGS
    PUBLIC_LIBRARIES
        # Remove: L../../bin/
        gamscpp
        user32
)

qt_extend_target(testgamsplatform CONDITION UNIX
    PUBLIC_LIBRARIES
        # Remove: L../../bin
        dl
        gamscpp
)

qt_extend_target(testgamsplatform CONDITION UNIX AND NOT MACOS
    PUBLIC_LIBRARIES
        stdc++fs
)

#### Keys ignored in scope 5:.:..:../tests.pri:NOT MACOS:
# GCCMAJORVERSION = "$$system("gcc -dumpversion")"

#### Keys ignored in scope 6:.:..:../tests.pri:(GCCMAJORVERSION LESS 9):
# QMAKE_CC = "gcc-9"
# QMAKE_CXX = "g++-9"

#### Keys ignored in scope 7:.:..:../tests.pri:MACOS:
# QMAKE_MACOSX_DEPLOYMENT_TARGET = "10.15"

#### Keys ignored in scope 8:.:..:../tests.pri:GOOGLETEST_DIR_ISEMPTY:
# GOOGLETEST_DIR = "$$(GOOGLETEST_DIR)"

#### Keys ignored in scope 11:.:..:../tests.pri:NOT GOOGLETEST_DIR_ISEMPTY:
# GMOCK_SRCDIR = "$$GOOGLETEST_DIR/googlemock"
# GTEST_SRCDIR = "$$GOOGLETEST_DIR/googletest"

#### Keys ignored in scope 14:.:..:../tests.pri:EXISTS /usr/src/gtest:
# GTEST_SRCDIR = "/usr/src/gtest"

#### Keys ignored in scope 15:.:..:../tests.pri:EXISTS /usr/src/gmock:
# GMOCK_SRCDIR = "/usr/src/gmock"

qt_extend_target(testgamsplatform CONDITION NOT GTEST_SRCDIR_ISEMPTY
    SOURCES
        /src/gtest-all.cc
    INCLUDE_DIRECTORIES
        /include
)

#### Keys ignored in scope 22:.:../..:../../gamsdependency.pri:MACOS:
# GAMSINC = "GAMS_DISTRIB=/Library/Frameworks/GAMS.framework/Versions/'$$GAMS_DISTRIB_MAJOR'/Resources" "GAMS_DISTRIB_C_API=\$$GAMS_DISTRIB/apifiles/C/api" "GAMS_DISTRIB_CPP_API=\$$GAMS_DISTRIB/apifiles/C++/api"

#### Keys ignored in scope 23:.:../..:../../gamsdependency.pri:UNIX AND NOT MACOS:
# GAMSINC = "GAMS_DISTRIB=$$(HOME)/gams/gams'$$GAMS_DISTRIB_MAJOR'.'$$GAMS_DISTRIB_MINOR'_linux_x64_64_sfx" "GAMS_DISTRIB_C_API=\$$GAMS_DISTRIB/apifiles/C/api" "GAMS_DISTRIB_CPP_API=\$$GAMS_DISTRIB/apifiles/C++/api"

#### Keys ignored in scope 24:.:../..:../../gamsdependency.pri:WIN32:
# GAMSINC = "GAMS_DISTRIB=C:/GAMS/win64/'$$GAMS_DISTRIB_MAJOR'.'$$GAMS_DISTRIB_MINOR'" "GAMS_DISTRIB_C_API=\$$GAMS_DISTRIB/apifiles/C/api" "GAMS_DISTRIB_CPP_API=\$$GAMS_DISTRIB/apifiles/C++/api"

#### Keys ignored in scope 25:.:../..:../../gamsdependency.pri:else:
# GAMSINC = "GAMS_DISTRIB=$$(GAMS_CORE_PATH)" "GAMS_DISTRIB_C_API=\$$GAMS_DISTRIB/apifiles/C/api" "GAMS_DISTRIB_CPP_API=\$$GAMS_DISTRIB/apifiles/C++/api"

qt_extend_target(testgamsplatform CONDITION GAMS_BUILD_ENV___equals___
    SOURCES
        /cfgmcc.c
        /gclgms.c
        /gcmt.c
        /gevmcc.c
        /gmdcc.c
        /gmomcc.c
        /optcc.c
)

qt_extend_target(testgamsplatform CONDITION NOT GAMS_BUILD_ENV___equals___
    SOURCES
        $ENV{GPRODUCTS}/apiwrap/gmdobj/gmdcc.c
        $ENV{GPRODUCTS}/apiwrap/joat/cfgmcc.c
        $ENV{GPRODUCTS}/apiwrap/joat/gevmcc.c
        $ENV{GPRODUCTS}/apiwrap/joat/gmomcc.c
        $ENV{GPRODUCTS}/apiwrap/optobj/optcc.c
        $ENV{GPRODUCTS}/gclib/gclgms.c
        $ENV{GPRODUCTS}/gclib/gcmt.c
    DEFINES
        HAVE_MUTEX
    INCLUDE_DIRECTORIES
        $ENV{GPRODUCTS}/apiwrap/gmdobj
        $ENV{GPRODUCTS}/apiwrap/joat
        $ENV{GPRODUCTS}/apiwrap/optobj
        $ENV{GPRODUCTS}/gclib
)

#### Keys ignored in scope 29:.:../..:../../gamsdependency.pri:else:
# GSYS_ENV = "$$(GSYS)"

qt_extend_target(testgamsplatform CONDITION GSYS_ENV___equals___wei AND NOT GAMS_BUILD_ENV___equals___
    DEFINES
        CIA_WEX
        WEI
)

qt_extend_target(testgamsplatform CONDITION GSYS_ENV___equals___vs8 AND NOT GAMS_BUILD_ENV___equals___
    DEFINES
        CIA_WIN
        VS8
)

qt_extend_target(testgamsplatform CONDITION GSYS_ENV___equals___leg AND NOT GAMS_BUILD_ENV___equals___
    DEFINES
        CIA_LEX
        LEG
    COMPILE_OPTIONS
        -Werror-implicit-function-declaration
        -Wmissing-declarations
        -Wreturn-type
        -m64
)

#### Keys ignored in scope 32:.:../..:../../gamsdependency.pri:GSYS_ENV___equals___leg:
# QMAKE_CFLAGS = "-Werror-implicit-function-declaration" "-Wreturn-type" "-Wmissing-prototypes" "-Wmissing-declarations" "-m64"

qt_extend_target(testgamsplatform CONDITION GSYS_ENV___equals___deg AND NOT GAMS_BUILD_ENV___equals___
    DEFINES
        CIA_DEX
        DEG
    COMPILE_OPTIONS
        -Werror-implicit-function-declaration
        -Wmissing-declarations
        -Wreturn-type
        -m64
)

#### Keys ignored in scope 33:.:../..:../../gamsdependency.pri:GSYS_ENV___equals___deg:
# QMAKE_CFLAGS = "-Werror-implicit-function-declaration" "-Wreturn-type" "-Wmissing-prototypes" "-Wmissing-declarations" "-m64"
