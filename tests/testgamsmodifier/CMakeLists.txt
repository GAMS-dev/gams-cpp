include(../include.cmake)
include_directories(. .. ../../src ${CMAKE_BINARY_DIR}/inc/)

set(SOURCE ${SOURCE} ../testgamsobject.cpp
                     ../../src/gamspath.cpp
                     testgamsmodifier.cpp)

add_executable(testgamsmodifier ${SOURCE})
target_link_libraries(testgamsmodifier gtest gamscpp)
add_test(testgamsmodifier testgamsmodifier)
