cmake_minimum_required(VERSION 3.17)

include(../include.cmake)

project(transport14 C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${GAMS_DISTRIB_CPP_API} PUBLIC_LIBRARIES)

add_executable(${PROJECT_NAME}
    "optimizer.h"
    "optimizer.cpp"
    "transport14.cpp")

if(WIN32)
target_link_libraries(${PROJECT_NAME} gamscpp)
elseif(UNIX)
target_link_libraries(${PROJECT_NAME} gamscpp pthread)
endif()
