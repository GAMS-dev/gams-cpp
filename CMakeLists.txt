#
# GAMS - General Algebraic Modeling System C++ API
#
# Copyright (c) 2017-2021 GAMS Software GmbH <support@gams.com>
# Copyright (c) 2017-2021 GAMS Development Corp. <support@gams.com>
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
set(GSYS_ENV $ENV{GSYS})
if ("${GSYS_ENV}" STREQUAL "deg")
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.14" CACHE STRING "Minimum OS X deployment version" FORCE)

    # use gcc on macos:
    set(CMAKE_C_COMPILER "/usr/local/gcc-10.3/bin/gcc" CACHE STRING "C Compiler" FORCE)
    set(CMAKE_CXX_COMPILER "/usr/local/gcc-10.3/bin/g++" CACHE STRING "C++ Compiler " FORCE)
endif()

project( GAMS-cpp )

cmake_minimum_required(VERSION 3.8)

include(version.cmake)

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_INCLUDE_CURRENT_DIR ON )
set( CMAKE_AUTOMOC OFF )

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
