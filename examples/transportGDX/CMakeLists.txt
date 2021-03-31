cmake_minimum_required(VERSION 3.8)

project(transportGDX C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
  message(STATUS "dir='${dir}'")
endforeach()

if ("$ENV{GAMS_BUILD}" STREQUAL "")
    message("RGDBG0.A: user git checkout")
    set(BASEPATH "${CMAKE_CURRENT_SOURCE_DIR}/../..")
else()
    message("RGDBG0.B: jenkins ${GAMSPATH}")
    set(BASEPATH "${GAMSPATH}/apifiles/")
endif()

include_directories("${BASEPATH}/C/api"
                    "${BASEPATH}/C++/api")

if(WIN32)
  set(VSVERSION "vs2019" CACHE STRING "Visual Studio version")
  link_directories("${BASEPATH}/C++/lib/${VSVERSION}")
else()
  link_directories("${BASEPATH}/C++/lib")
endif()

add_executable(${PROJECT_NAME} "transportGDX.cpp")

target_link_libraries(${PROJECT_NAME} gamscpp)
