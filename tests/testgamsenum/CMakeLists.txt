include(../include.cmake)
include_directories(. .. ../../src ${CMAKE_BINARY_DIR}/inc/)

set(SOURCE ${SOURCE} ../testgamsobject.cpp
                     ../../src/gamspath.cpp
                     testgamsenum.cpp)

add_executable(testgamsenum ${SOURCE})
target_link_libraries(testgamsenum gtest gamscpp)
add_test(testgamsenum testgamsenum)
