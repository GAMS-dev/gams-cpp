cmake_minimum_required(VERSION 2.8.11)

project(transport1 C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/../../gamsinclude.pri" GAMSINCLUDE LIMIT_COUNT 1)
message(${GAMSINCLUDE})

string(REGEX MATCH "[^=]+$" GAMSPATH ${GAMSINCLUDE})
message(GAMSPATH: ${VALUE})

# TODO(RG) the following only works for distrib, not when checking gamscpp out independently
INCLUDE_DIRECTORIES("${GAMSPATH}/apifiles/C/api"
    "${GAMSPATH}/apifiles/C++/api")

IF (WIN32)
  SET(VSVERSION "vs2017" CACHE STRING "Visual Studio version") # TODO(RG): change to 2019?
  LINK_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/../lib/${VSVERSION}")
ELSE()
  LINK_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/../lib")
ENDIF()

add_executable(${PROJECT_NAME} "transport1.cpp")

TARGET_LINK_LIBRARIES(${PROJECT_NAME} gamscpp)

