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

cmake_minimum_required(VERSION 3.17)

project( GAMS-cpp )

if (NOT "$ENV{GAMS_BUILD}" STREQUAL "")
    if ("${GSYS_ENV}" STREQUAL "leg")
        set(OPENSSL_ROOT_DIR "$ENV{BTREE}/openssl/leg/build")
        set(OPENSSL_INCLUDE_DIR "$ENV{BTREE}/openssl/leg/include")
        set(OPENSSL_CRYPTO_LIBRARY "$ENV{BTREE}/openssl/leg/lib/libcrypto.so")
        # work around for caching issue in OpenSSL checks
        set(CPR_PATCH_COMMAND sed -i -e /cmake_minimum_required/s/3.15/3.17/ CMakeLists.txt)
    endif()
    if (NOT "${MACOS_LEGACY_ENV}" STREQUAL "")
        # switch to OpenSSL, because macOS 12 headers do not compile with GCC
        # (https://gcc.gnu.org/bugzilla/show_bug.cgi?id=93082)
        set(CPR_FORCE_OPENSSL_BACKEND ON)
        set(CMAKE_USE_SECTRANSP OFF)
        # overwrite wrong detection of __builtin_available by curl
        set(HAVE_BUILTIN_AVAILABLE 0)
        # allow warnings in libcpr: remove -Werror, or just the whole line
        # (cpr-src/cpr/util.cpp:20: error: "__STDC_WANT_LIB_EXT1__" redefined [-Werror])
        set(CPR_PATCH_COMMAND sed -i -e /Werror/d CMakeLists.txt)
    endif()
endif()

include(FetchContent)

# Add Cpr: cpr is a modern HTTP library for C++ (MIT License)
FetchContent_Declare(cpr URL https://github.com/libcpr/cpr/archive/refs/tags/1.10.5.tar.gz PATCH_COMMAND ${CPR_PATCH_COMMAND})
FetchContent_MakeAvailable(cpr)

# add nlohmann:json to handle json format (MIT License)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
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
