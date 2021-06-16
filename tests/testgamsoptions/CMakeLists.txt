include(../include.cmake)
include_directories(. .. ../../src ../../src/generated)

set(SOURCE ${SOURCE} ../testgamsobject.cpp
                     ../../src/gamspath.cpp
                     testgamsoptions.cpp)

add_executable(testgamsoptions ${SOURCE})
target_link_libraries(testgamsoptions gtest gamscpp)
add_test(testgamsoptions testgamsoptions)
