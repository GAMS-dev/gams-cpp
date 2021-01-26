#
# GAMS - General Algebraic Modeling System C++ API
#
# Copyright (c) 2017-2020 GAMS Software GmbH <support@gams.com>
# Copyright (c) 2017-2020 GAMS Development Corp. <support@gams.com>
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
set (GITCHECKOUT true) # TODO(RG): use this?
set (NO_WINDOWS_REGISTRY ${NO_WINDOWS_REGISTRY})
# include gamsdependency.pri # TODO(RG): add this

# TODO(RG): read this https://www.cs.swarthmore.edu/~adanner/tips/cmake.php

if (WIN32)
    message(${ORIGIN})
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        add_definitions(-EHsc -GR -c -nologo -DFNAME_UCASE_NODECOR -DF_CALLCONV=__cdecl -D_CRT_DISABLE_PERFCRIT_LOCKS)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "G++") # TODO(RG): find out how to check for G++: win32-g++
        add_definitions(-DFNAME_UCASE_NODECOR -DF_CALLCONV=__cdecl -D_CRT_DISABLE_PERFCRIT_LOCKS)
    endif()
    add_library(user32)

elseif (UNIX) # Apple or Linux
    add_library(dl)
    if (APPLE)
        set(MACOSX_DEPLOYMENT_TARGET 10.15)
    else() # UNIX
        add_definitions(-Wl,-rpath,\'\$$ORIGIN\',-rpath,\'\$$ORIGIN/../../..\') # TODO(RG): check this
        link_libraries(stdc++fs)
        if(CMAKE_CXX_COMPILER_ID STREQUAL "G++")
            execute_process(COMMAND "gcc -dumpversion" OUTPUT_VARIABLE GCCMAJORVERSION)
            if ($GCCMAJORVERSION LESS 9)
                set(CC "gcc-9")
                set(CXX "g++-9")
            endif()
        endif()
    endif()
endif()

INCLUDE_DIRECTORIES (../src/generated PUBLIC_LIBRARIES)
#    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../bin" # TODO(RG): do we need this?
add_library(gamscpp
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
        ../CHANGELOG ../version
)
include(../CMakeInclude.txt)

#### Keys ignored in scope 1:.:.:gamscpp.pro:<TRUE>:
# OTHER_FILES = "gamscpp.qmodel" "coreclasses.qmodel" "../CHANGELOG" "../jenkinsfile" "../version"
# TEMPLATE = "lib"
# USE_GAMS_DISTRIB_CPP_API_SRC = "false"

## Scopes:
#####################################################################

#### Keys ignored in scope 15:.:..:../gamsdependency.pri:MACOS:
# GAMSINC = "GAMS_DISTRIB=/Library/Frameworks/GAMS.framework/Versions/'$$GAMS_DISTRIB_MAJOR'/Resources" "GAMS_DISTRIB_C_API=\$$GAMS_DISTRIB/apifiles/C/api" "GAMS_DISTRIB_CPP_API=\$$GAMS_DISTRIB/apifiles/C++/api"

#### Keys ignored in scope 16:.:..:../gamsdependency.pri:UNIX AND NOT MACOS:
# GAMSINC = "GAMS_DISTRIB=$$(HOME)/gams/gams'$$GAMS_DISTRIB_MAJOR'.'$$GAMS_DISTRIB_MINOR'_linux_x64_64_sfx" "GAMS_DISTRIB_C_API=\$$GAMS_DISTRIB/apifiles/C/api" "GAMS_DISTRIB_CPP_API=\$$GAMS_DISTRIB/apifiles/C++/api"

#### Keys ignored in scope 17:.:..:../gamsdependency.pri:WIN32:
# GAMSINC = "GAMS_DISTRIB=C:/GAMS/win64/'$$GAMS_DISTRIB_MAJOR'.'$$GAMS_DISTRIB_MINOR'" "GAMS_DISTRIB_C_API=\$$GAMS_DISTRIB/apifiles/C/api" "GAMS_DISTRIB_CPP_API=\$$GAMS_DISTRIB/apifiles/C++/api"

#### Keys ignored in scope 18:.:..:../gamsdependency.pri:else:
# GAMSINC = "GAMS_DISTRIB=$$(GAMS_CORE_PATH)" "GAMS_DISTRIB_C_API=\$$GAMS_DISTRIB/apifiles/C/api" "GAMS_DISTRIB_CPP_API=\$$GAMS_DISTRIB/apifiles/C++/api"

#qt_extend_target(gamscpp CONDITION GAMS_BUILD_ENV___equals___
#    SOURCES
#        /cfgmcc.c
#        /gclgms.c
#        /gcmt.c
#        /gevmcc.c
#        /gmdcc.c
#        /gmomcc.c
#        /optcc.c
#)

#qt_extend_target(gamscpp CONDITION NOT GAMS_BUILD_ENV___equals___
#    SOURCES
#        $ENV{GPRODUCTS}/apiwrap/gmdobj/gmdcc.c
#        $ENV{GPRODUCTS}/apiwrap/joat/cfgmcc.c
#        $ENV{GPRODUCTS}/apiwrap/joat/gevmcc.c
#        $ENV{GPRODUCTS}/apiwrap/joat/gmomcc.c
#        $ENV{GPRODUCTS}/apiwrap/optobj/optcc.c
#        $ENV{GPRODUCTS}/gclib/gclgms.c
#        $ENV{GPRODUCTS}/gclib/gcmt.c
#    DEFINES
#        HAVE_MUTEX
#    INCLUDE_DIRECTORIES
#        $ENV{GPRODUCTS}/apiwrap/gmdobj
#        $ENV{GPRODUCTS}/apiwrap/joat
#        $ENV{GPRODUCTS}/apiwrap/optobj
#        $ENV{GPRODUCTS}/gclib
#)

##### Keys ignored in scope 22:.:..:../gamsdependency.pri:else:
## GSYS_ENV = "$$(GSYS)"

#qt_extend_target(gamscpp CONDITION GSYS_ENV___equals___wei AND NOT GAMS_BUILD_ENV___equals___
#    DEFINES
#        CIA_WEX
#        WEI
#)

#qt_extend_target(gamscpp CONDITION GSYS_ENV___equals___vs8 AND NOT GAMS_BUILD_ENV___equals___
#    DEFINES
#        CIA_WIN
#        VS8
#)

#qt_extend_target(gamscpp CONDITION GSYS_ENV___equals___leg AND NOT GAMS_BUILD_ENV___equals___
#    DEFINES
#        CIA_LEX
#        LEG
#    COMPILE_OPTIONS
#        -Werror-implicit-function-declaration
#        -Wmissing-declarations
#        -Wreturn-type
#        -m64
#)

##### Keys ignored in scope 25:.:..:../gamsdependency.pri:GSYS_ENV___equals___leg:
## QMAKE_CFLAGS = "-Werror-implicit-function-declaration" "-Wreturn-type" "-Wmissing-prototypes" "-Wmissing-declarations" "-m64"

#qt_extend_target(gamscpp CONDITION GSYS_ENV___equals___deg AND NOT GAMS_BUILD_ENV___equals___
#    DEFINES
#        CIA_DEX
#        DEG
#    COMPILE_OPTIONS
#        -Werror-implicit-function-declaration
#        -Wmissing-declarations
#        -Wreturn-type
#        -m64
#)

##### Keys ignored in scope 26:.:..:../gamsdependency.pri:GSYS_ENV___equals___deg:
## QMAKE_CFLAGS = "-Werror-implicit-function-declaration" "-Wreturn-type" "-Wmissing-prototypes" "-Wmissing-declarations" "-m64"

#qt_autogen_tools_initial_setup(gamscpp)
