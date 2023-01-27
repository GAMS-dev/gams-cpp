set (NO_WINDOWS_REGISTRY ${NO_WINDOWS_REGISTRY})

set(VERSION ${VERSION} PARENT_SCOPE)
include(../include.cmake)

set(GAMSPATH ${GAMSPATH} PARENT_SCOPE) # this is needed to make this var available to siblings
add_definitions(-DMAKELIB)

if(WIN32)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        add_definitions(-EHsc -GR -c -nologo -DFNAME_UCASE_NODECOR -DF_CALLCONV=__cdecl -D_CRT_DISABLE_PERFCRIT_LOCKS)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        add_definitions(-DFNAME_UCASE_NODECOR -DF_CALLCONV=__cdecl -D_CRT_DISABLE_PERFCRIT_LOCKS)
    endif()

elseif(UNIX) # Apple or Linux
    link_libraries(dl)
    if (NOT APPLE)
        set(CMAKE_SHARED_LINKER_FLAGS "-Wl,-rpath,$ORIGIN,-rpath,$ORIGIN/../../..")
        link_libraries(stdc++fs pthread)
    endif()
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 8)
            set(CMAKE_C_COMPILER "gcc-8")
            set(CMAKE_CXX_COMPILER "g++-8")
        endif()
    endif()
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(${CMAKE_BINARY_DIR}/inc)

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
        gamsenginejob.cpp gamsenginejob.h
        gamsengineconfiguration.cpp gamsengineconfiguration.h
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
        solveroptions.cpp solveroptions.h
        ../CHANGELOG ../version
        ${SOURCE} ${HEADER}
    )
