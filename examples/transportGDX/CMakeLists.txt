cmake_minimum_required(VERSION 3.8)

project(transportGDX C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT $ENV{GAMS_BUILD})
    set(BASEPATH "${CMAKE_CURRENT_SOURCE_DIR}/../..")
else()
    set(BASEPATH "${GAMSPATH}/apifiles/")
endif()
include_directories("${BASEPATH}/C/api"
                    "${BASEPATH}/C++/api"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../src"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../src/generated")

if(WIN32)
  set(VSVERSION "vs2019" CACHE STRING "Visual Studio version")
  link_directories("${BASEPATH}/C++/lib/${VSVERSION}")
else()
  link_directories("${BASEPATH}/C++/lib")
endif()

add_executable(${PROJECT_NAME} "transportGDX.cpp")

target_link_libraries(${PROJECT_NAME} gamscpp)
