cmake_minimum_required(VERSION 3.8)

project(transport8 C CXX)

include(../include.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${GAMS_DISTRIB_CPP_API} PUBLIC_LIBRARIES)

add_executable(${PROJECT_NAME}
    "transport8.cpp")

if(WIN32)
target_link_libraries(${PROJECT_NAME} gamscpp)
elseif(UNIX)
target_link_libraries(${PROJECT_NAME} gamscpp pthread)
endif()
