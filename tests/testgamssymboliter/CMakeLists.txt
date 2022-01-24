include(../include.cmake)
include_directories(. .. ../../src ${CMAKE_BINARY_DIR}/inc/)

set(SOURCE ${SOURCE} ../testgamsobject.cpp
                     ../../src/gamspath.cpp
                     testgamssymboliter.cpp)

add_executable(testgamssymboliter ${SOURCE})
target_link_libraries(testgamssymboliter gtest gamscpp)
add_test(testgamssymboliter testgamssymboliter)
