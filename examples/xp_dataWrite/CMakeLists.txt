cmake_minimum_required(VERSION 3.1)

project(xp_dataWrite C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/../../C/api")

add_executable(${PROJECT_NAME}
    "xp_dataWrite.cpp"
    "../../C/api/gdxcc.c"
    "../../C/api/gcmt.c"
    "../../C/api/gmdcc.c")

IF (WIN32)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} user32)
ELSEIF(UNIX)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} dl pthread)
ENDIF()
