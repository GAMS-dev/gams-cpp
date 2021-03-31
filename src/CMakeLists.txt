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
set (NO_WINDOWS_REGISTRY ${NO_WINDOWS_REGISTRY})
include(../include.cmake)

set(GAMSPATH ${GAMSPATH} PARENT_SCOPE) # this is needed to make this var available to siblings
add_definitions(-DMAKELIB)

if(WIN32)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        add_definitions(-EHsc -GR -c -nologo -DFNAME_UCASE_NODECOR -DF_CALLCONV=__cdecl -D_CRT_DISABLE_PERFCRIT_LOCKS)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "G++") # TODO(RG): find out how to check for G++: win32-g++
        add_definitions(-DFNAME_UCASE_NODECOR -DF_CALLCONV=__cdecl -D_CRT_DISABLE_PERFCRIT_LOCKS)
    endif()

elseif(UNIX) # Apple or Linux
    link_libraries(dl)
    set(CMAKE_SHARED_LINKER_FLAGS "-Wl,-rpath,${ORIGIN},-rpath,${ORIGIN}/../../..")
    if (APPLE)
        message("RGDBG is Apple")
        link_libraries(c++fs pthread)
    else()
        message("RGDBG is Unix w/o apple: ${CMAKE_CXX_COMPILER_ID}")
        link_libraries(stdc++fs pthread)
        if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9)
                set(CMAKE_C_COMPILER "gcc-9")
                set(CMAKE_CXX_COMPILER "g++-9")
            endif()
        endif()
    endif()
endif()

# TODO(RG):  we need to find a solution for how to handle generated files...
include_directories(../src/generated PUBLIC_LIBRARIES)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_library(gamscpp SHARED
        gams.h
        gamscheckpoint.cpp gamscheckpoint.h
        gamscheckpointimpl.cpp gamscheckpointimpl.h
        gamsdatabase.cpp gamsdatabase.h
        gamsdatabasedomainviolation.cpp gamsdatabasedomainviolation.h
        gamsdatabasedomainviolationimpl.cpp gamsdatabasedomainviolationimpl.h
        gamsdatabaseimpl.cpp gamsdatabaseimpl.h
        gamsdatabaseiter.cpp gamsdatabaseiter.h
        gamsdomain.cpp gamsdomain.h
        gamsdomainimpl.cpp gamsdomainimpl.h
        gamsenum.cpp gamsenum.h
        gamsequation.cpp gamsequation.h
        gamsequationrecord.cpp gamsequationrecord.h
        gamsexception.cpp gamsexception.h
        gamsexceptionexecution.cpp gamsexceptionexecution.h
        gamsjob.cpp gamsjob.h
        gamsjobimpl.cpp gamsjobimpl.h
        gamslib_global.h
        gamslog.cpp gamslog.h
        gamsmodelinstance.cpp gamsmodelinstance.h
        gamsmodelinstanceimpl.cpp gamsmodelinstanceimpl.h gamsmodelinstanceimpl_p.cpp
        gamsmodelinstanceopt.cpp gamsmodelinstanceopt.h
        gamsmodelinstanceoptimpl.cpp gamsmodelinstanceoptimpl.h
        gamsmodifier.cpp gamsmodifier.h
        gamsmodifierimpl.cpp gamsmodifierimpl.h
        gamsparameter.cpp gamsparameter.h
        gamsparameterrecord.cpp gamsparameterrecord.h
        gamspath.cpp gamspath.h
        gamsplatform.cpp gamsplatform.h
        gamsset.cpp gamsset.h
        gamssetrecord.cpp gamssetrecord.h
        gamssymbol.cpp gamssymbol.h
        gamssymboldomainviolation.cpp gamssymboldomainviolation.h
        gamssymboldomainviolationimpl.cpp gamssymboldomainviolationimpl.h
        gamssymbolimpl.cpp gamssymbolimpl.h gamssymbolimpl_p.cpp
        gamssymboliter.cpp gamssymboliter.h
        gamssymbolrecord.cpp gamssymbolrecord.h
        gamssymbolrecordimpl.cpp gamssymbolrecordimpl.h
        gamsvariable.cpp gamsvariable.h
        gamsvariablerecord.cpp gamsvariablerecord.h
        gamsversion.cpp gamsversion.h
        gamsworkspace.cpp gamsworkspace.h
        gamsworkspaceimpl.cpp gamsworkspaceimpl.h
        gamsworkspaceinfo.cpp gamsworkspaceinfo.h
        gamsworkspaceinfoimpl.cpp gamsworkspaceinfoimpl.h
        gamsworkspacepool.cpp gamsworkspacepool.h
        generated/gamsoptions.cpp
        generated/gamsoptionsimpl.cpp
        solveroptions.cpp solveroptions.h
        ../CHANGELOG ../version
        ${SOURCE} ${HEADER}
)

