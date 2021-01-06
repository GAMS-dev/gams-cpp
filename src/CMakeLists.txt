# Generated from gamscpp.pro.

#####################################################################
## gamscpp Generic Library:
#####################################################################

qt_add_cmake_library(gamscpp
    MODULE
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../bin"
    SOURCES
        gams.h
        gamscheckpoint.cpp gamscheckpoint.h
        gamscheckpointimpl.cpp gamscheckpointimpl.h
        gamsdatabase.cpp gamsdatabase.h
        gamsdatabasedomainviolation.cpp gamsdatabasedomainviolation.h
        gamsdatabasedomainviolationimpl.cpp gamsdatabasedomainviolationimpl.h
        gamsdatabaseimpl.cpp gamsdatabaseimpl.h
        gamsdatabaseiter.cpp gamsdatabaseiter.h
        gamsdomain.cpp gamsdomain.h
        gamsdomainimpl.cpp gamsdomainimpl.h
        gamsenum.cpp gamsenum.h
        gamsequation.cpp gamsequation.h
        gamsequationrecord.cpp gamsequationrecord.h
        gamsexception.cpp gamsexception.h
        gamsexceptionexecution.cpp gamsexceptionexecution.h
        gamsjob.cpp gamsjob.h
        gamsjobimpl.cpp gamsjobimpl.h
        gamslib_global.cpp gamslib_global.h
        gamslog.cpp gamslog.h
        gamsmodelinstance.cpp gamsmodelinstance.h
        gamsmodelinstanceimpl.cpp gamsmodelinstanceimpl.h gamsmodelinstanceimpl_p.cpp
        gamsmodelinstanceopt.cpp gamsmodelinstanceopt.h
        gamsmodelinstanceoptimpl.cpp gamsmodelinstanceoptimpl.h
        gamsmodifier.cpp gamsmodifier.h
        gamsmodifierimpl.cpp gamsmodifierimpl.h
        gamsparameter.cpp gamsparameter.h
        gamsparameterrecord.cpp gamsparameterrecord.h
        gamspath.cpp gamspath.h
        gamsplatform.cpp gamsplatform.h
        gamsset.cpp gamsset.h
        gamssetrecord.cpp gamssetrecord.h
        gamssymbol.cpp gamssymbol.h
        gamssymboldomainviolation.cpp gamssymboldomainviolation.h
        gamssymboldomainviolationimpl.cpp gamssymboldomainviolationimpl.h
        gamssymbolimpl.cpp gamssymbolimpl.h gamssymbolimpl_p.cpp
        gamssymboliter.cpp gamssymboliter.h
        gamssymbolrecord.cpp gamssymbolrecord.h
        gamssymbolrecordimpl.cpp gamssymbolrecordimpl.h
        gamsvariable.cpp gamsvariable.h
        gamsvariablerecord.cpp gamsvariablerecord.h
        gamsversion.cpp gamsversion.h
        gamsworkspace.cpp gamsworkspace.h
        gamsworkspaceimpl.cpp gamsworkspaceimpl.h
        gamsworkspaceinfo.cpp gamsworkspaceinfo.h
        gamsworkspaceinfoimpl.cpp gamsworkspaceinfoimpl.h
        gamsworkspacepool.cpp gamsworkspacepool.h
        generated/gamsoptions.cpp
        generated/gamsoptionsimpl.cpp
        solveroptions.cpp solveroptions.h
    DEFINES
        -UNICODE
        API_VERSION=\\\"'0'.'11'.'0'\\\"
        GAMS_CPP_LIB
        MAKELIB
    INCLUDE_DIRECTORIES
        ../src/generated
    PUBLIC_LIBRARIES
        Qt::Core
)

#### Keys ignored in scope 1:.:.:gamscpp.pro:<TRUE>:
# NO_WINDOWS_REGISTRY = "$$(NO_WINDOWS_REGISTRY)"
# OTHER_FILES = "gamscpp.qmodel" "coreclasses.qmodel" "../CHANGELOG" "../jenkinsfile" "../version"
# TEMPLATE = "lib"
# USE_GAMS_DISTRIB_CPP_API_SRC = "false"

## Scopes:
#####################################################################

qt_extend_target(gamscpp CONDITION NO_WINDOWS_REGISTRY___equals___1
    DEFINES
        NO_WINDOWS_REGISTRY
)

qt_extend_target(gamscpp CONDITION WIN32
    DEFINES
        _CRT_SECURE_NO_WARNINGS
    PUBLIC_LIBRARIES
        user32
)

qt_extend_target(gamscpp CONDITION (WIN32) AND (win32-g++)
    DEFINES
        FNAME_UCASE_NODECOR
        F_CALLCONV=__cdecl
        _CRT_DISABLE_PERFCRIT_LOCKS
)

qt_extend_target(gamscpp CONDITION WIN32 AND win32-msvc_x_
    DEFINES
        FNAME_UCASE_NODECOR
        F_CALLCONV=__cdecl
        _CRT_DISABLE_PERFCRIT_LOCKS
    COMPILE_OPTIONS
        -EHsc
        -GR
        -c
        -nologo
)

qt_extend_target(gamscpp CONDITION UNIX
    PUBLIC_LIBRARIES
        dl
)

qt_extend_target(gamscpp CONDITION UNIX AND NOT MACOS
    PUBLIC_LIBRARIES
        stdc++fs
    LINK_OPTIONS
        "-Wl,-rpath,'$$ORIGIN',-rpath,'$$ORIGIN/../../..'"
)

#### Keys ignored in scope 8:.:.:gamscpp.pro:linux-g++:
# GCCMAJORVERSION = "$$system("gcc -dumpversion")"

#### Keys ignored in scope 9:.:.:gamscpp.pro:(GCCMAJORVERSION LESS 9):
# QMAKE_CC = "gcc-9"
# QMAKE_CXX = "g++-9"

#### Keys ignored in scope 10:.:.:gamscpp.pro:MACOS:
# QMAKE_MACOSX_DEPLOYMENT_TARGET = "10.15"

#### Keys ignored in scope 15:.:..:../gamsdependency.pri:MACOS:
# GAMSINC = "GAMS_DISTRIB=/Library/Frameworks/GAMS.framework/Versions/'$$GAMS_DISTRIB_MAJOR'/Resources" "GAMS_DISTRIB_C_API=\$$GAMS_DISTRIB/apifiles/C/api" "GAMS_DISTRIB_CPP_API=\$$GAMS_DISTRIB/apifiles/C++/api"

#### Keys ignored in scope 16:.:..:../gamsdependency.pri:UNIX AND NOT MACOS:
# GAMSINC = "GAMS_DISTRIB=$$(HOME)/gams/gams'$$GAMS_DISTRIB_MAJOR'.'$$GAMS_DISTRIB_MINOR'_linux_x64_64_sfx" "GAMS_DISTRIB_C_API=\$$GAMS_DISTRIB/apifiles/C/api" "GAMS_DISTRIB_CPP_API=\$$GAMS_DISTRIB/apifiles/C++/api"

#### Keys ignored in scope 17:.:..:../gamsdependency.pri:WIN32:
# GAMSINC = "GAMS_DISTRIB=C:/GAMS/win64/'$$GAMS_DISTRIB_MAJOR'.'$$GAMS_DISTRIB_MINOR'" "GAMS_DISTRIB_C_API=\$$GAMS_DISTRIB/apifiles/C/api" "GAMS_DISTRIB_CPP_API=\$$GAMS_DISTRIB/apifiles/C++/api"

#### Keys ignored in scope 18:.:..:../gamsdependency.pri:else:
# GAMSINC = "GAMS_DISTRIB=$$(GAMS_CORE_PATH)" "GAMS_DISTRIB_C_API=\$$GAMS_DISTRIB/apifiles/C/api" "GAMS_DISTRIB_CPP_API=\$$GAMS_DISTRIB/apifiles/C++/api"

qt_extend_target(gamscpp CONDITION GAMS_BUILD_ENV___equals___
    SOURCES
        /cfgmcc.c
        /gclgms.c
        /gcmt.c
        /gevmcc.c
        /gmdcc.c
        /gmomcc.c
        /optcc.c
)

qt_extend_target(gamscpp CONDITION NOT GAMS_BUILD_ENV___equals___
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

#### Keys ignored in scope 22:.:..:../gamsdependency.pri:else:
# GSYS_ENV = "$$(GSYS)"

qt_extend_target(gamscpp CONDITION GSYS_ENV___equals___wei AND NOT GAMS_BUILD_ENV___equals___
    DEFINES
        CIA_WEX
        WEI
)

qt_extend_target(gamscpp CONDITION GSYS_ENV___equals___vs8 AND NOT GAMS_BUILD_ENV___equals___
    DEFINES
        CIA_WIN
        VS8
)

qt_extend_target(gamscpp CONDITION GSYS_ENV___equals___leg AND NOT GAMS_BUILD_ENV___equals___
    DEFINES
        CIA_LEX
        LEG
    COMPILE_OPTIONS
        -Werror-implicit-function-declaration
        -Wmissing-declarations
        -Wreturn-type
        -m64
)

#### Keys ignored in scope 25:.:..:../gamsdependency.pri:GSYS_ENV___equals___leg:
# QMAKE_CFLAGS = "-Werror-implicit-function-declaration" "-Wreturn-type" "-Wmissing-prototypes" "-Wmissing-declarations" "-m64"

qt_extend_target(gamscpp CONDITION GSYS_ENV___equals___deg AND NOT GAMS_BUILD_ENV___equals___
    DEFINES
        CIA_DEX
        DEG
    COMPILE_OPTIONS
        -Werror-implicit-function-declaration
        -Wmissing-declarations
        -Wreturn-type
        -m64
)

#### Keys ignored in scope 26:.:..:../gamsdependency.pri:GSYS_ENV___equals___deg:
# QMAKE_CFLAGS = "-Werror-implicit-function-declaration" "-Wreturn-type" "-Wmissing-prototypes" "-Wmissing-declarations" "-m64"

qt_autogen_tools_initial_setup(gamscpp)
