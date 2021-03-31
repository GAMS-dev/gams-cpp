cmake_minimum_required(VERSION 3.8)

project(transport5 C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT DEFINED ${GAMS_BUILD_ENV})
    set(BASEPATH "${CMAKE_CURRENT_SOURCE_DIR}/../..")
else()
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

add_executable(${PROJECT_NAME}
    "transport5.cpp")

target_link_libraries(${PROJECT_NAME} gamscpp)
