include(../include.cmake)
include_directories(. .. ../../src ${CMAKE_BINARY_DIR}/inc/)

set(SOURCE ${SOURCE} ../testgamsobject.cpp
                     ../../src/gamspath.cpp
                     testgamsworkspace.cpp)

add_executable(testgamsworkspace ${SOURCE})
target_link_libraries(testgamsworkspace gtest gamscpp)
add_test(testgamsworkspace testgamsworkspace)
