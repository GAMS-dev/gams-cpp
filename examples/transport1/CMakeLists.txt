cmake_minimum_required(VERSION 3.1)

project(transport1 C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (IS_IN_GAMS_DIR)
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../C/api"
                        "${CMAKE_CURRENT_SOURCE_DIR}/../api")
    set(PATH ${CMAKE_CURRENT_SOURCE_DIR})
else()
    include_directories("${GAMSPATH}/apifiles/C/api"
                        "${GAMSPATH}/apifiles/C++/api")
endif()

if(WIN32)
  set(VSVERSION "vs2019" CACHE STRING "Visual Studio version") # TODO(RG): change to 2019?
  message("TRANSPORT1 ${GAMSPATH}")
  link_directories("${GAMSPATH}/apifiles/C++/lib/${VSVERSION}")
else()
  link_directories("${GAMSPATH}/apifiles/C++/lib")
endif()

add_executable(${PROJECT_NAME} "transport1.cpp")

target_link_libraries(${PROJECT_NAME} gamscpp)

