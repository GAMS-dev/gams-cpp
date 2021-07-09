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
if(WIN32)
    # Switch off warnings caused by GAMS headers
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# Project file version defines
add_definitions(-DAPI_VERSION="${VERSION}")

# GAMS_CORE_PATH is Jenkins build switch
file(TO_CMAKE_PATH "$ENV{GAMS_CORE_PATH}" GAMS_PATH)
if(NOT EXISTS ${PWD}/gamsinclude.pri)
    if("${GAMS_PATH}" STREQUAL "")
        if(APPLE)
            set(GAMS_DISTRIB /Library/Frameworks/GAMS.framework/Versions/${GAMS_DISTRIB_MAJOR}/Resources)
            set(GAMSINC
"GAMS_DISTRIB=${GAMS_DISTRIB}\n\
GAMS_DISTRIB_C_API=${GAMS_DISTRIB}/apifiles/C/api\n\
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
    else()
        set(GAMSINC
"GAMS_DISTRIB=${GAMS_PATH}\n\
GAMS_DISTRIB_C_API=${GAMS_PATH}/apifiles/C/api\n\
GAMS_DISTRIB_CPP_API=${GAMS_PATH}/apifiles/C++/api")
    endif()

    file(WRITE ${PWD}/gamsinclude.pri ${GAMSINC})
endif()

include(../version.cmake)
if(EXISTS ${PWD}/gamsinclude.pri)
    ReadFromFileAndSet(${PWD}/gamsinclude.pri)
endif()

# create variable GAMSPATH from gamsinclude.pri
if("$ENV{GAMS_BUILD}" STREQUAL "")
    file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/../gamsinclude.pri" GAMSINCLUDE LIMIT_COUNT 1)
    string(REGEX MATCH "[^=]+$" GAMSPATH ${GAMSINCLUDE})
else()
    set(GAMSPATH "${CMAKE_CURRENT_SOURCE_DIR}/../../..")
endif()

# GAMS_BUILD is GAMS distrib build switch
if("$ENV{GAMS_BUILD}" STREQUAL "")
    include_directories(${GAMS_DISTRIB_C_API})

    set(SOURCE ${SOURCE} ${GAMS_DISTRIB_C_API}/gclgms.c
                         ${GAMS_DISTRIB_C_API}/gmdcc.c
                         ${GAMS_DISTRIB_C_API}/gcmt.c
                         ${GAMS_DISTRIB_C_API}/cfgmcc.c
                         ${GAMS_DISTRIB_C_API}/gevmcc.c
                         ${GAMS_DISTRIB_C_API}/gmomcc.c
                         ${GAMS_DISTRIB_C_API}/optcc.c)


     configure_file(${GAMS_DISTRIB_CPP_API}/gamsoptionsimpl.cpp ${CMAKE_BINARY_DIR}/inc/gamsoptionsimpl.cpp COPYONLY)
     configure_file(${GAMS_DISTRIB_CPP_API}/gamsoptionsimpl.h ${CMAKE_BINARY_DIR}/inc/gamsoptionsimpl.h COPYONLY)
     configure_file(${GAMS_DISTRIB_CPP_API}/gamsoptions.cpp ${CMAKE_BINARY_DIR}/inc/gamsoptions.cpp COPYONLY)
     configure_file(${GAMS_DISTRIB_CPP_API}/gamsoptions.h ${CMAKE_BINARY_DIR}/inc/gamsoptions.h COPYONLY)

     set(SOURCE ${SOURCE} ${CMAKE_BINARY_DIR}/inc/gamsoptionsimpl.cpp ${CMAKE_BINARY_DIR}/inc/gamsoptions.cpp )
     set(HEADER ${HEADER} ${CMAKE_BINARY_DIR}/inc/gamsoptionsimpl.h ${CMAKE_BINARY_DIR}/inc/gamsoptions.h)
else()
    set(GSYS_ENV $ENV{GSYS})

    if ("${GSYS_ENV}" STREQUAL "leg")
        add_definitions(-DLEG -DCIA_LEX)
    endif()

    if ("${GSYS_ENV}" STREQUAL "deg")
        add_definitions(-DDEG -DCIA_DEX)
        set(CMAKE_SHARED_LINKER_FLAGS "-Wl,-rpath,$ORIGIN,-rpath,$ORIGIN/../../..")
        link_libraries(stdc++fs pthread)
    endif()

    set(GPRODUCTS_ENV $ENV{GPRODUCTS})
    include_directories(${GPRODUCTS_ENV}/gclib
                        ${GPRODUCTS_ENV}/apiwrap/gmdobj
                        ${GPRODUCTS_ENV}/apiwrap/joat
                        ${BTREE}/optobj/${GSYS})

    set(SOURCE ${SOURCE} ${GPRODUCTS_ENV}/gclib/gclgms.c
                         ${GPRODUCTS_ENV}/gclib/gcmt.c
                         ${GPRODUCTS_ENV}/apiwrap/gmdobj/gmdcc.c
                         ${GPRODUCTS_ENV}/apiwrap/joat/cfgmcc.c
                         ${GPRODUCTS_ENV}/apiwrap/joat/gevmcc.c
                         ${GPRODUCTS_ENV}/apiwrap/joat/gmomcc.c
                         ${BTREE}/optobj/optcc.c)

    include_directories(${GPRODUCTS_ENV}/apiexamples/C++/api)
    set(SOURCE ${SOURCE} ${GPRODUCTS_ENV}/apiexamples/C++/api/gamsoptions.cpp
                         ${GPRODUCTS_ENV}/apiexamples/C++/api/gamsoptionsimpl.cpp)
endif()

