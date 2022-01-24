include(../include.cmake)
include_directories(. .. ../../src ${CMAKE_BINARY_DIR}/inc/)

set(SOURCE ${SOURCE} ../testgamsobject.cpp
                     ../../src/gamspath.cpp
                     testgamsequation.cpp)

add_executable(testgamsequation ${SOURCE})
target_link_libraries(testgamsequation gtest gamscpp)
add_test(testgamsequation testgamsequation)
