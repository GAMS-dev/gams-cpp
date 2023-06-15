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

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_INCLUDE_CURRENT_DIR ON )

if (APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15" CACHE STRING "Minimum OS X deployment version" FORCE)
    set(CMAKE_C_COMPILER "/usr/bin/gcc")
    set(CMAKE_CXX_COMPILER "/usr/bin/g++")
endif()

cmake_minimum_required(VERSION 3.11)

project( GAMS-cpp )

message("ROGO DEBUG INFO:")
message("CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
message("CMAKE_CXX_COMPILER_VERSION: ${CMAKE_CXX_COMPILER_VERSION}")

# Add Cpr: cpr is a modern HTTP library for C++ (MIT License)
include(FetchContent)
FetchContent_Declare(cpr URL https://github.com/libcpr/cpr/archive/refs/tags/1.10.2.tar.gz)
FetchContent_MakeAvailable(cpr)

# add nlohmann:json to handle json format (MIT License)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

include(version.cmake)

add_subdirectory( src )
add_subdirectory( examples )

set(PACKAGE_TESTS ON)
if(PACKAGE_TESTS)
    enable_testing()
    if(WIN32)
        set(BUILD_SHARED_LIBS ON)
    endif()
    add_subdirectory(tests)
endif()

