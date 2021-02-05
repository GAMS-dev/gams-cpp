cmake_minimum_required(VERSION 2.8.11)

project(transport1 C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# TODO(RG): improve this!
if (IS_IN_GAMS_DIR)
    INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/../../C/api"
                        "${CMAKE_CURRENT_SOURCE_DIR}/../api")
    set(PATH ${CMAKE_CURRENT_SOURCE_DIR})
else()
    INCLUDE_DIRECTORIES("${GAMSPATH}/apifiles/C/api"
                        "${GAMSPATH}/apifiles/C++/api")
endif()

IF (WIN32)
  SET(VSVERSION "vs2017" CACHE STRING "Visual Studio version") # TODO(RG): change to 2019?
  LINK_DIRECTORIES("${GAMSPATH}/apifiles/C++/lib/${VSVERSION}")
ELSE()
  LINK_DIRECTORIES("${GAMSPATH}/apifiles/C++/lib")
ENDIF()

add_executable(${PROJECT_NAME} "transport1.cpp")

TARGET_LINK_LIBRARIES(${PROJECT_NAME} gamscpp)

