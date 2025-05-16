set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

if("${CMAKE_SOURCE_DIR}" MATCHES "apifiles/C\\+\\+$")
    # build examples ony (GAMS distrib)
    if("${GAMSPATH}" STREQUAL "")
        set(BASEPATH "${CMAKE_CURRENT_SOURCE_DIR}/../..")
    else()
        set(BASEPATH "${GAMSPATH}/apifiles")
    endif()

    # check if run from distrib
    if(NOT EXISTS "${CMAKE_SOURCE_DIR}/src")
        if(WIN32)
            set(VSVERSION "vs2022" CACHE STRING "Visual Studio version")
            link_directories("${BASEPATH}/C++/lib/${VSVERSION}")
        elseif(APPLE)
            if(USE-GCC STREQUAL "ON" OR "$ENV{GSYS}" STREQUAL "dac")
                link_directories("${BASEPATH}/C++/lib")
            else()
                link_directories("${BASEPATH}/C++/lib/clang")
            endif()
        else()
            link_directories("${BASEPATH}/C++/lib")
        endif()
    endif()

    include_directories("${BASEPATH}/C/api"
                        "${CMAKE_SOURCE_DIR}/src"
                        "${BASEPATH}/C++/api")
elseif(NOT "$ENV{GAMS_BUILD}" STREQUAL "")
    set(BASEPATH "${GAMSPATH}/apiexamples")
    if(UNIX)
        add_link_options("-Wl,-rpath,$ENV{BTREE}/gmszlib/leg")
    endif()
    include_directories("${BASEPATH}/../gclib"
                        "${CMAKE_SOURCE_DIR}/src"
                        "${CMAKE_SOURCE_DIR}/C++/api"
                        "${BASEPATH}/C++/api")
else()
    # GitLab or GitHub stand alone subproject
    if("${GAMSPATH}" STREQUAL "")
        set(BASEPATH "${CMAKE_CURRENT_SOURCE_DIR}/../..")
    else()
        set(BASEPATH "${GAMSPATH}/apifiles")
    endif()

    configure_file(${BASEPATH}/C++/api/gamsoptionsimpl.cpp
      ${PROJECT_SOURCE_DIR}/inc/gamsoptionsimpl.cpp COPYONLY)
    configure_file(${BASEPATH}/C++/api/gamsoptionsimpl.h
      ${PROJECT_SOURCE_DIR}/inc/gamsoptionsimpl.h COPYONLY)
    configure_file(${BASEPATH}/C++/api/gamsoptions.cpp
      ${PROJECT_SOURCE_DIR}/inc/gamsoptions.cpp COPYONLY)
    configure_file(${BASEPATH}/C++/api/gamsoptions.h
      ${PROJECT_SOURCE_DIR}/inc/gamsoptions.h COPYONLY)
    set(SOURCE ${SOURCE} ${PROJECT_SOURCE_DIR}/inc/gamsoptionsimpl.cpp
      ${PROJECT_SOURCE_DIR}/inc/gamsoptions.cpp)

    include_directories("${BASEPATH}/C/api"
                        "${CMAKE_SOURCE_DIR}/src"
                        "${PROJECT_SOURCE_DIR}/inc")
endif()

if(UNIX) # Apple or Linux
    link_libraries(dl)
    if(NOT APPLE)
        link_libraries(stdc++fs pthread)
    endif()
endif()
message("#### ${BASEPATH}")
add_link_options("-Wl,-rpath,${BASEPATH}/..")
