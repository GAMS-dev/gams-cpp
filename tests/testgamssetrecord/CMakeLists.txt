include(../include.cmake)
include_directories(. .. ../../src ${CMAKE_BINARY_DIR}/inc/)

set(SOURCE ${SOURCE} ../testgamsobject.cpp
                     ../../src/gamspath.cpp
                     testgamssetrecord.cpp)

add_executable(testgamssetrecord ${SOURCE})
target_link_libraries(testgamssetrecord gtest gamscpp)
add_test(testgamssetrecord testgamssetrecord)
