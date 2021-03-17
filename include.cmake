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
set(PWD ${CMAKE_CURRENT_SOURCE_DIR}/..)

function(ReadFromFileAndSet FILEPATH)
    file(STRINGS ${FILEPATH} FileContents)
    foreach(NameAndValue ${FileContents})

        # Strip leading spaces
        string(REGEX REPLACE "^[ ]+" "" NameAndValue ${NameAndValue})

        # ignore comments
        if(${NameAndValue} MATCHES "^#")
            continue()
        endif()

        # Find variable name
        string(REGEX MATCH "^[^=]+" Name ${NameAndValue})
        # Find the value
        string(REPLACE "${Name}=" "" Value ${NameAndValue})
        # Set the variable
        set(${Name} "${Value}" PARENT_SCOPE)
    endforeach()
endfunction()

if(WIN32)
    # Switch off warnings caused by GAMS headers
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

ReadFromFileAndSet(${PWD}/version)
set(VERSION "${GAMSCPP_MAJOR_VERSION}.${GAMSCPP_MINOR_VERSION}.${GAMSCPP_PATCH_LEVEL}")

# Project file version defines
add_definitions(-DAPI_VERSION="${VERSION}")

# GAMS_CORE_PATH is Jenkins build switch
set(GAMS_CORE_TMP ${GAMS_CORE_PATH})
if(NOT EXISTS ${PWD}/gamsinclude.pri)
    if(NOT DEFINED ${GAMS_CORE_TMP})
        if(APPLE)
            set(GAMS_DISTRIB /Library/Frameworks/GAMS.framework/Versions/${GAMS_DISTRIB_MAJOR}/Resources)
            set(GAMSINC
"GAMS_DISTRIB=${GAMS_DISTRIB}\n\
GAMS_DISTRIB_C_API=${GAMS_DISTRIB}/apifiles/C/api\
GAMS_DISTRIB_CPP_API=${GAMS_DISTRIB}/apifiles/C++/api")

        elseif(UNIX AND NOT APPLE)
            set(GAMS_DISTRIB $ENV{HOME}/gams/gams${GAMS_DISTRIB_MAJOR}.${GAMS_DISTRIB_MINOR}_linux_x64_64_sfx)
            set(GAMSINC
"GAMS_DISTRIB=${GAMS_DISTRIB}\n\
GAMS_DISTRIB_C_API=${GAMS_DISTRIB}/apifiles/C/api\n\
GAMS_DISTRIB_CPP_API=${GAMS_DISTRIB}/apifiles/C++/api")

        elseif(WIN32)
            set(GAMS_DISTRIB C:/GAMS/${GAMS_DISTRIB_MAJOR})
            set(GAMSINC
"GAMS_DISTRIB=${GAMS_DISTRIB}\n\
GAMS_DISTRIB_C_API=${GAMS_DISTRIB}/apifiles/C/api\n\
GAMS_DISTRIB_CPP_API=${GAMS_DISTRIB}/apifiles/C++/api")
        endif()

        file(WRITE ${PWD}/gamsinclude.pri ${GAMSINC})
    elseif()
        set(GAMSINC
"GAMS_DISTRIB=${GAMS_CORE_PATH}\n\
GAMS_DISTRIB_C_API=${GAMS_DISTRIB}/apifiles/C/api\n\
GAMS_DISTRIB_CPP_API=${GAMS_DISTRIB}/apifiles/C++/api")
        file(WRITE ${PWD}/gamsinclude.pri ${GAMSINC})
    endif()
endif()

if(EXISTS ${PWD}/gamsinclude.pri)
    ReadFromFileAndSet(${PWD}/gamsinclude.pri)
endif()

# GAMS_BUILD is GAMS distrib build switch
if(NOT DEFINED ${GAMS_BUILD_ENV})
    include_directories(${GAMS_DISTRIB_C_API})

    set(SOURCE ${SOURCE} ${GAMS_DISTRIB_C_API}/gclgms.c
                         ${GAMS_DISTRIB_C_API}/gmdcc.c
                         ${GAMS_DISTRIB_C_API}/gcmt.c
                         ${GAMS_DISTRIB_C_API}/cfgmcc.c
                         ${GAMS_DISTRIB_C_API}/gevmcc.c
                         ${GAMS_DISTRIB_C_API}/gmomcc.c
                         ${GAMS_DISTRIB_C_API}/optcc.c)

    if(${USE_GAMS_DISTRIB_CPP_API_SRC})
        set(HEADER ${HEADER} ${GAMS_DISTRIB_CPP_API}/gamsoptionsimpl.h
                             ${GAMS_DISTRIB_CPP_API}/gamsoptions.h
                             PARENT_SCOPE)
        set(SOURCE ${SOURCE} ${GAMS_DISTRIB_CPP_API}/gamsoptions.cpp
                             ${GAMS_DISTRIB_CPP_API}/gamsoptionsimpl.cpp
                             PARENT_SCOPE)
    endif()
else()
    add_definitions(-DHAVE_MUTEX)
    set(GSYS_ENV ${GSYS})
    if (${GSYS_ENV} STREQUALS "wei")
        add_definitions(WEI CIA_WEX)
    endif()

    if (${GSYS_ENV} STREQUALS "vs8")
        add_definitions(vs8 CIA_WIN)
    endif()

    if (${GSYS_ENV} STREQUALS "leg")
        add_definitions(LEG CIA_LEX)
        add_definitions(-Werror-implicit-function-declaration -Wreturn-type -Wmissing-declarations -m64)
        add_definitions(-Werror-implicit-function-declaration -Wreturn-type -Wmissing-prototypes -Wmissing-declarations -m64)
        # original:
#         QMAKE_CXXFLAGS += -Werror-implicit-function-declaration -Wreturn-type -Wmissing-declarations -m64
#         QMAKE_CFLAGS += -Werror-implicit-function-declaration -Wreturn-type -Wmissing-prototypes -Wmissing-declarations -m64
    endif()

    if (${GSYS_ENV} STREQUALS "deg")
        add_definitions(DEG CIA_DEX)
        add_definitions(-Werror-implicit-function-declaration -Wreturn-type -Wmissing-declarations -m64)
        add_definitions(-Werror-implicit-function-declaration -Wreturn-type -Wmissing-prototypes -Wmissing-declarations -m64)
    endif()

    include_directories(${GPRODUCTS}/gclib
                        ${GPRODUCTS}/apiwrap/gmdobj
                        ${GPRODUCTS}/apiwrap/joat
                        ${GPRODUCTS}/apiwrap/optobj)

    set(SOURCE ${SOURCE} ${GPRODUCTS}/gclib/gclgms.c
                         ${GPRODUCTS}/gclib/gcmt.c
                         ${GPRODUCTS}/apiwrap/gmdobj/gmdcc.c
                         ${GPRODUCTS}/apiwrap/joat/cfgmcc.c
                         ${GPRODUCTS}/apiwrap/joat/gevmcc.c
                         ${GPRODUCTS}/apiwrap/joat/gmomcc.c
                         ${GPRODUCTS}/apiwrap/optobj/optcc.c)

    if (${USE_GAMS_DISTRIB_CPP_API_SRC})
        include_directories(${GPRODUCTS}/apiexamples/C++/api)
        set(SOURCE ${SOURCE} ${GPRODUCTS}/apiexamples/C++/api/gamsoptions.cpp
                             ${GPRODUCTS}/apiexamples/C++/api/gamsoptionsimpl.cpp)
    endif()
endif()
# create variable GAMSPATH from gamsinclude.pri
file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/../gamsinclude.pri" GAMSINCLUDE LIMIT_COUNT 1)
string(REGEX MATCH "[^=]+$" GAMSPATH ${GAMSINCLUDE})
