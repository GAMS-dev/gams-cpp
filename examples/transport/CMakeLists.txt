cmake_minimum_required(VERSION 2.8)

project(Transport1 C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("../../C/api"
    "../api")



add_executable(${PROJECT_NAME}
    "main.cpp"
    "transport1.h"
    "transport1.cpp")

if(WIN32)
target_link_libraries(${PROJECT_NAME} gamscpp)
elseif(UNIX)
target_link_libraries(${PROJECT_NAME} gamscpp)
endif()
