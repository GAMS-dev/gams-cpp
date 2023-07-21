set(PWD ${CMAKE_CURRENT_SOURCE_DIR}/..)
if(WIN32)
    # Switch off warnings caused by GAMS headers
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# Project file version defines
add_definitions(-DAPI_VERSION="${VERSION}")

# GAMS_CORE_PATH is Jenkins build switch
file(TO_CMAKE_PATH "$ENV{GAMS_CORE_PATH}" GAMS_PATH)
if(NOT EXISTS ${PWD}/gamsinclude.txt)
    if("${GAMS_PATH}" STREQUAL "")
        if(APPLE)
            set(GAMS_DISTRIB /Library/Frameworks/GAMS.framework/Versions/${GAMS_DISTRIB_MAJOR}/Resources)
            set(GAMSINC "GAMS_DISTRIB=${GAMS_DISTRIB}")

        elseif(UNIX AND NOT APPLE)
            set(GAMS_DISTRIB $ENV{HOME}/gams/gams${GAMS_DISTRIB_MAJOR}.${GAMS_DISTRIB_MINOR}_linux_x64_64_sfx)
            set(GAMSINC "GAMS_DISTRIB=${GAMS_DISTRIB}")

        elseif(WIN32)
            set(GAMS_DISTRIB C:/GAMS/${GAMS_DISTRIB_MAJOR})
            set(GAMSINC "GAMS_DISTRIB=${GAMS_DISTRIB}")
        endif()
    else()
        set(GAMSINC "GAMS_DISTRIB=${GAMS_PATH}")
    endif()

    file(WRITE ${PWD}/gamsinclude.txt ${GAMSINC})
endif()

include(../version.cmake)
if(EXISTS ${PWD}/gamsinclude.txt)
    ReadFromFileAndSet(${PWD}/gamsinclude.txt)
endif()
set(GAMS_DISTRIB_C_API "${GAMS_DISTRIB}/apifiles/C/api")
set(GAMS_DISTRIB_CPP_API "${GAMS_DISTRIB}/apifiles/C++/api")

# create variable GAMSPATH from gamsinclude.txt
if("$ENV{GAMS_BUILD}" STREQUAL "")
    if (NOT EXISTS ${GAMS_DISTRIB_C_API})
        message(FATAL_ERROR "No GAMS found in path ${GAMS_DISTRIB}. GAMS is required to compile the C++ API.
            Please adjust ${CMAKE_SOURCE_DIR}/gamsinclude.txt to point to your GAMS installation.")
    endif()
    file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/../gamsinclude.txt" GAMSINCLUDE LIMIT_COUNT 1)
    string(REGEX MATCH "[^=]+$" GAMSPATH ${GAMSINCLUDE})
else()
    set(GAMSPATH "${CMAKE_CURRENT_SOURCE_DIR}/../../..")
endif()

# GAMS_BUILD is GAMS distrib build switch
if("$ENV{GAMS_BUILD}" STREQUAL "")
    include_directories(${GAMS_DISTRIB_C_API})

    set(SOURCE ${SOURCE} ${GAMS_DISTRIB_C_API}/gmdcc.c
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
    set(MACOS_LEGACY_ENV $ENV{MACOS_LEGACY})

    if ("${GSYS_ENV}" STREQUAL "leg")
        add_definitions(-DLEG -DCIA_LEX)
    endif()

    if ("${GSYS_ENV}" STREQUAL "deg")
        add_definitions(-DDEG -DCIA_DEX)
        if ("${MACOS_LEGACY_ENV}" STREQUAL "true")
            set(CMAKE_SHARED_LINKER_FLAGS "-Wl,-rpath,$ORIGIN,-rpath,$ORIGIN/../../..")
            link_libraries(stdc++fs pthread)
        endif()
    endif()

    set(GPRODUCTS_ENV $ENV{GPRODUCTS})
    set(BTREE_GMD_ENV $ENV{BTREE}/gmdxxx/$ENV{GSYS})
    set(BTREE_JOAT_ENV $ENV{BTREE}/joat/$ENV{GSYS})
    set(BTREE_OPT_ENV $ENV{BTREE}/optobj/$ENV{GSYS})
    include_directories(${GPRODUCTS_ENV}/gclib
                        ${BTREE_GMD_ENV}
                        ${BTREE_JOAT_ENV}
                        ${BTREE_OPT_ENV})

    set(SOURCE ${SOURCE} ${GPRODUCTS_ENV}/gclib/gcmt.c
                         ${BTREE_GMD_ENV}/gmdcc.c
                         ${BTREE_JOAT_ENV}/cfgmcc.c
                         ${BTREE_JOAT_ENV}/gevmcc.c
                         ${BTREE_JOAT_ENV}/gmomcc.c
                         ${BTREE_OPT_ENV}/optcc.c)

    include_directories(${GPRODUCTS_ENV}/apiexamples/C++/api)
    set(SOURCE ${SOURCE} ${GPRODUCTS_ENV}/apiexamples/C++/api/gamsoptions.cpp
                         ${GPRODUCTS_ENV}/apiexamples/C++/api/gamsoptionsimpl.cpp)
endif()

