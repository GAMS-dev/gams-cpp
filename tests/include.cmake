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
link_libraries(../../bin/gamscpp)

if(UNIX) # Apple or Linux
    link_libraries(dl)
    if (APPLE)
        set(MACOSX_DEPLOYMENT_TARGET 10.15)
    else() # UNIX
        set(CMAKE_SHARED_LINKER_FLAGS "-Wl,-rpath,${ORIGIN},-rpath,${ORIGIN}/../../..")
        link_libraries(stdc++fs pthread)
        if (CMAKE_CXX_COMPILER_ID STREQUAL "G++")
            execute_process(COMMAND "gcc -dumpversion" OUTPUT_VARIABLE GCCMAJORVERSION)
            if (${GCCMAJORVERSION} LESS 9)
                set(CC "gcc-9")
                set(CXX "g++-9")
            endif()
        endif()
    endif()
endif()

# GoogleTest
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../src)
if(NOT DEFINED ${GOOGLETEST_DIR})
    set(GOOGLETEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../googletest")
endif()

set(GTEST_SRCDIR ${GOOGLETEST_DIR}/googletest)
include_directories(${GTEST_SRCDIR} ${GTEST_SRCDIR}/include)
enable_testing()


