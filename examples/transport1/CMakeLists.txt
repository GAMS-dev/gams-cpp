cmake_minimum_required(VERSION 3.8)

project(transport1 C CXX)

include(../include.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("${BASEPATH}/C/api"
                    "${BASEPATH}/C++/api"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../src")
include_directories(${GAMS_DISTRIB_CPP_API} PUBLIC_LIBRARIES)

add_executable(${PROJECT_NAME} "transport1.cpp")

target_link_libraries(${PROJECT_NAME} gamscpp)

