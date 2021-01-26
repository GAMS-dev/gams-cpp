cmake_minimum_required(VERSION 2.8.11)

project(transport1 C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# TODO(RG): experimental changes, revert at some point!
file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/../../gamsinclude.pri" GAMSINCLUDE LIMIT_COUNT 1)
message(${GAMSINCLUDE})

string(REGEX MATCH "[^=]+$" GAMSPATH ${GAMSINCLUDE})
message(GAMSPATH: ${VALUE})

INCLUDE_DIRECTORIES("${GAMSPATH}/apifiles/C/api"
    "${GAMSPATH}/apifiles/C++/api")

message(${GAMSPATH})

IF (WIN32)
  SET(VSVERSION "vs2017" CACHE STRING "Visual Studio version") # TODO(RG): change to 2019?
  LINK_DIRECTORIES("${GAMSPATH}/apifiles/C++/lib/${VSVERSION}")
ELSE()
  LINK_DIRECTORIES("${GAMSPATH}/apifiles/C++/lib")
ENDIF()

add_executable(${PROJECT_NAME} "transport1.cpp")

TARGET_LINK_LIBRARIES(${PROJECT_NAME} gamscpp)

