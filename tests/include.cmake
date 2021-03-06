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
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)
if ("$ENV{GAMS_BUILD}" STREQUAL "")
    set(BASEPATH "${GAMSPATH}/apifiles")
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../C++/api")
else()
    # jenkins switch:
    if("$ENV{GAMS_CORE_PATH}" STREQUAL "")
        set(BASEPATH "${GAMSPATH}/apiexamples")
        include_directories("${BASEPATH}/C++/api")
    else()
        set(BASEPATH "${GAMSPATH}/apifiles")
    endif()
endif()

include_directories("${BASEPATH}/C/api"
                    "${gtest_SOURCE_DIR}/include")

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
