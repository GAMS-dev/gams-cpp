cmake_minimum_required(VERSION 3.17)

include(../include.cmake)

project(transport2 C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${GAMS_DISTRIB_CPP_API} PUBLIC_LIBRARIES)

add_executable(${PROJECT_NAME} "transport2.cpp")

target_link_libraries(${PROJECT_NAME} gamscpp)
