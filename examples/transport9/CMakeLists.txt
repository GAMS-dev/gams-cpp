cmake_minimum_required(VERSION 2.8.11)

project(transport9 C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/../../C/api"
    "${CMAKE_CURRENT_SOURCE_DIR}/../api")

IF (WIN32)
  SET(VSVERSION "vs2017" CACHE STRING "Visual Studio version")
  LINK_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/../lib/${VSVERSION}")
ELSE()
  LINK_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/../lib")
ENDIF()

add_executable(${PROJECT_NAME}
    "transport9.cpp"
    )

IF (WIN32)
    find_package(Qt5Core)
    qt5_use_modules(${PROJECT_NAME} Core Sql)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} gamscpp)
ELSEIF(UNIX)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} gamscpp)
ENDIF()