include(../include.cmake)
include_directories(. .. ../../src ${CMAKE_BINARY_DIR}/inc/)

set(SOURCE ${SOURCE} ../testgamsobject.cpp
                     ../../src/gamspath.cpp
                     testgamssymbolrecord.cpp)

add_executable(testgamssymbolrecord ${SOURCE})
target_link_libraries(testgamssymbolrecord gtest gamscpp)
add_test(testgamssymbolrecord testgamssymbolrecord)
