set(GSYS_ENV $ENV{GSYS})
set(MACOS_LEGACY_ENV $ENV{MACOS_LEGACY})
if ("${GSYS_ENV}" STREQUAL "deg")
    if ("${MACOS_LEGACY_ENV}" STREQUAL "")
        set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15" CACHE STRING "Minimum OS X deployment version" FORCE)
    else()
        # use gcc for macos targets:
        set(CMAKE_OSX_DEPLOYMENT_TARGET "10.14" CACHE STRING "Minimum OS X deployment version" FORCE)
        set(CMAKE_C_COMPILER "$ENV{SYSLIBDIR}/../bin/gcc" CACHE STRING "C Compiler" FORCE)
        set(CMAKE_CXX_COMPILER "$ENV{SYSLIBDIR}/../bin/g++" CACHE STRING "C++ Compiler " FORCE)
    endif()
endif()

project( GAMS-cpp )

cmake_minimum_required(VERSION 3.8)

include(version.cmake)

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_INCLUDE_CURRENT_DIR ON )
set( CMAKE_AUTOMOC OFF )
set( CPR_ENABLE_SSL OFF ) # TODO(RG): add openSSL to build system

# Add Cpr: cpr is a modern HTTP library for C++
include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git GIT_TAG master)
FetchContent_MakeAvailable(cpr)

link_libraries(cpr::cpr)

add_subdirectory( src )
add_subdirectory( examples )

# TODO(RG): reactivate tests
set(PACKAGE_TESTS OFF)
if(PACKAGE_TESTS)
    enable_testing()
    if(WIN32)
        set(BUILD_SHARED_LIBS ON)
    endif()
    add_subdirectory(tests)
endif()

