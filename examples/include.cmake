set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

if ("$ENV{GAMS_BUILD}" STREQUAL "")
    # stand alone subproject
    if ("${GAMSPATH}" STREQUAL "")
        set(BASEPATH "${CMAKE_CURRENT_SOURCE_DIR}/../..")
    else()
        set(BASEPATH "${GAMSPATH}/apifiles")
    endif()

    # check if run from distrib
    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src")
        if(WIN32)
            set(VSVERSION "vs2019" CACHE STRING "Visual Studio version")
            link_directories("${BASEPATH}/C++/lib/${VSVERSION}")
        else()
            link_directories("${BASEPATH}/C++/lib")
        endif()
    endif()

    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../src"
                        "${CMAKE_CURRENT_SOURCE_DIR}/../../C++/api"
                        "${BASEPATH}/C/api"
                        "${BASEPATH}/C++/api")
else()
    set(BASEPATH "${GAMSPATH}/apiexamples")
    include_directories("${BASEPATH}/C++/api"
                        "${CMAKE_CURRENT_SOURCE_DIR}/../../src"
                        "${CMAKE_CURRENT_SOURCE_DIR}/../../C++/api"
                        "${BASEPATH}/../gclib")
    if(UNIX)
        add_link_options("-Wl,-rpath,$ENV{BTREE}/gmszlib/leg")
    endif()
endif()


if(UNIX) # Apple or Linux
    link_libraries(dl)
    if (NOT APPLE)
        link_libraries(stdc++fs pthread)
    endif()
endif()
