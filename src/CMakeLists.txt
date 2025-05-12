set(NO_WINDOWS_REGISTRY ${NO_WINDOWS_REGISTRY})

set(VERSION ${VERSION} PARENT_SCOPE)
include(../include.cmake)

# this is needed to make this var available to siblings
set(GAMSPATH ${GAMSPATH} PARENT_SCOPE)
add_definitions(-DMAKELIB)

add_compile_options(-Wall)
#add_compile_options(-Wall -Wextra -pedantic -Werror)

if(WIN32)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        add_definitions(-EHsc -GR -c -nologo -DFNAME_UCASE_NODECOR
          -DF_CALLCONV=__cdecl -D_CRT_DISABLE_PERFCRIT_LOCKS)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        add_definitions(-DFNAME_UCASE_NODECOR -DF_CALLCONV=__cdecl
          -D_CRT_DISABLE_PERFCRIT_LOCKS)
    endif()
elseif(UNIX) # Apple or Linux
    link_libraries(dl)
    if(NOT APPLE)
        set(CMAKE_SHARED_LINKER_FLAGS
          "-Wl,-rpath,$ORIGIN,-rpath,$ORIGIN/../../..")
        link_libraries(stdc++fs pthread)
    endif()
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 8)
            set(CMAKE_C_COMPILER "gcc-8")
            set(CMAKE_CXX_COMPILER "g++-8")
        endif()
    endif()
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(${CMAKE_BINARY_DIR}/inc)

# add Cpr: cpr is a modern HTTP library for C++ (MIT License)
include(FetchContent)
FetchContent_Declare(cpr URL
  https://github.com/libcpr/cpr/archive/refs/tags/1.11.2.tar.gz)
FetchContent_MakeAvailable(cpr)

# add nlohmann:json to handle json format (MIT License)
FetchContent_Declare(json URL
  https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)

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
  ../ci/.gitlab-ci-0-quality-gate.yml
  ../ci/.gitlab-ci-1-build.yml
  ../ci/.gitlab-ci-2-test.yml
  ../ci/.gitlab-ci-3-deploy.yml
  ../ci/.gitlab-ci-4-analyze.yml
  ../ci/codechecker.sh
  ../ci/license-update.sh
  ../ci/upload-release.sh
  ../.gitlab-ci.yml
  ../gamsinclude.txt
  ../.gitignore
  ../.pre-commit-config.yaml
  ../.secrets.baseline
  ../CHANGELOG
  ../CONTRIBUTING.md
  ../README.md
  ../version
  ${SOURCE} ${HEADER}
)

target_link_libraries(gamscpp cpr::cpr)
target_link_libraries(gamscpp nlohmann_json::nlohmann_json)
