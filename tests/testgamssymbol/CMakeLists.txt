include(../include.cmake)
include_directories(. .. ../../src ${CMAKE_BINARY_DIR}/inc/)

set(SOURCE ${SOURCE} ../testgamsobject.cpp
                     ../../src/gamspath.cpp
                     testgamssymbol.cpp)

add_executable(testgamssymbol ${SOURCE})
target_link_libraries(testgamssymbol gtest gamscpp)
add_test(testgamssymbol testgamssymbol)
