set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)
if ("$ENV{GAMS_BUILD}" STREQUAL "")
    set(BASEPATH "${GAMSPATH}/apifiles")
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../C++/api"
                        "${BASEPATH}/C/api"
                        "${gtest_SOURCE_DIR}/include")
else()
    # jenkins switch:
    if("$ENV{GAMS_CORE_PATH}" STREQUAL "")
        set(BASEPATH "${GAMSPATH}/apiexamples")
        include_directories("${BASEPATH}/C++/api")
    else()
        set(BASEPATH "${GAMSPATH}/apifiles")
    endif()
    include_directories("${BASEPATH}/../gclib"
                        "${gtest_SOURCE_DIR}/include")
endif()

if(WIN32)
    set(VSVERSION "vs2019" CACHE STRING "Visual Studio version")
    link_directories("${BASEPATH}/C++/lib/${VSVERSION}")
else()
    link_directories("${BASEPATH}/C++/lib")
endif()

if(UNIX) # Apple or Linux
    link_libraries(dl)
    if (NOT APPLE)
        link_libraries(stdc++fs pthread)
    endif()
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 8)
            set(CMAKE_C_COMPILER "gcc-8")
            set(CMAKE_CXX_COMPILER "g++-8")
        endif()
    endif()
endif()
