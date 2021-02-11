cmake_minimum_required(VERSION 3.1)

project(transport9 C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../C/api"
    "${CMAKE_CURRENT_SOURCE_DIR}/../api")

if(WIN32)
  set(VSVERSION "vs2019" CACHE STRING "Visual Studio version")
  link_directories("${CMAKE_CURRENT_SOURCE_DIR}/../lib/${VSVERSION}")
else()
  link_directories("${CMAKE_CURRENT_SOURCE_DIR}/../lib")
endif()

add_executable(${PROJECT_NAME}
    "transport9.cpp"
    )

if(WIN32)
    find_package(Qt5Core)
    qt5_use_modules(${PROJECT_NAME} Core Sql)
    target_link_libraries(${PROJECT_NAME} gamscpp)
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME} gamscpp)
endif()
