cmake_minimum_required(VERSION 3.1)

project(transport13 C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/../../C/api"
    "${CMAKE_CURRENT_SOURCE_DIR}/../api"
    "${CMAKE_CURRENT_SOURCE_DIR}/../transport")

IF (WIN32)
  SET(VSVERSION "vs2017" CACHE STRING "Visual Studio version")
  LINK_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/../lib/${VSVERSION}")
ELSE()
  LINK_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/../lib")
ENDIF()

add_executable(${PROJECT_NAME}
    "transport13.cpp"
    "../transport/transport.cpp")

TARGET_LINK_LIBRARIES(${PROJECT_NAME} gamscpp)
