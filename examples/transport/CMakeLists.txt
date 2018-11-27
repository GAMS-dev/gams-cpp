cmake_minimum_required(VERSION 2.8)

project(Transport1 C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

INCLUDE_DIRECTORIES("../../C/api"
    "../api")

IF (WIN32)
  SET(VSVERSION "vs2017" CACHE STRING "Visual Studio version")
  LINK_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/../lib/${VSVERSION}")
ELSE()
  LINK_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/../lib")
ENDIF()

add_executable(${PROJECT_NAME}
    "main.cpp"
    "transport1.h"
    "transport1.cpp")

IF (WIN32)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} gamscpp)
ELSEIF(UNIX)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} gamscpp)
ENDIF()
