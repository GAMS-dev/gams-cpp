cmake_minimum_required(VERSION 3.1)

project(xp_example2 C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (IS_IN_GAMS_DIR)
    set(BASEPATH "${CMAKE_CURRENT_SOURCE_DIR}/../..")
else()
    set(BASEPATH "${GAMSPATH}/apifiles/")
endif()
include_directories("${BASEPATH}/C/api"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../src"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../../src/generated")

add_executable(${PROJECT_NAME}
    "xp_example2.cpp"
    "${BASEPATH}/C/api/gcmt.c"
    "${BASEPATH}/C/api/gdxcc.c"
    "${BASEPATH}/C/api/optcc.c"
    "${BASEPATH}/C/api/gamsxcc.c")

if(WIN32)
target_link_libraries(${PROJECT_NAME} user32)
elseif(UNIX)
target_link_libraries(${PROJECT_NAME} dl pthread)
endif()
