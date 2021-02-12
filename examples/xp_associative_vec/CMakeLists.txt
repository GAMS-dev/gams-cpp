cmake_minimum_required(VERSION 3.1)

project(xp_associative_vec C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (IS_IN_GAMS_DIR)
    set(BASEPATH "${CMAKE_CURRENT_SOURCE_DIR}/../..")
else()
    set(BASEPATH "${GAMSPATH}/apifiles/")
endif()
include_directories("${BASEPATH}/C/api")

add_executable(${PROJECT_NAME}
    "xp_associative_vec.cpp"
    "${BASEPATH}/C/api/gcmt.c"
    "${BASEPATH}/C/api/gdxcc.c")

if(WIN32)
target_link_libraries(${PROJECT_NAME} user32)
elseif(UNIX)
target_link_libraries(${PROJECT_NAME} dl pthread)
endif()
