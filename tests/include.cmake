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
if (NOT DEFINED $ENV{GAMS_BUILD})
    set(BASEPATH "${CMAKE_CURRENT_SOURCE_DIR}/../..")
else()
    set(BASEPATH "${GAMSPATH}/apifiles/")
endif()

include_directories("${GAMSPATH}/apifiles/C/api"
                    "${BASEPATH}/C++/api"
                    "${gtest_SOURCE_DIR}/include")

if(WIN32)
    set(VSVERSION "vs2019" CACHE STRING "Visual Studio version")
    link_directories("${BASEPATH}/C++/lib/${VSVERSION}")
else()
    link_directories("${BASEPATH}/C++/lib")
endif()

if(UNIX) # Apple or Linux
#    link_libraries(dl) # TODO(RG): this can maybe be removed
    if (APPLE)
        set(MACOSX_DEPLOYMENT_TARGET 10.15)
    else() # UNIX
# TODO(RG): the following can probably be removed
#        set(CMAKE_SHARED_LINKER_FLAGS "-Wl,-rpath,${ORIGIN},-rpath,${ORIGIN}/../../..")
        link_libraries(stdc++fs pthread)
        if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9)
                set(CMAKE_C_COMPILER "gcc-9")
                set(CMAKE_CXX_COMPILER "g++-9")
            endif()
        endif()
    endif()
endif()
