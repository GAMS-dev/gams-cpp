include(../include.cmake)
include_directories(. .. ../../src ${CMAKE_BINARY_DIR}/inc/)

set(SOURCE ${SOURCE} ../testgamsobject.cpp
                     ../../src/gamspath.cpp
                     testgamsset.cpp)

add_executable(testgamsset ${SOURCE})
target_link_libraries(testgamsset gtest gamscpp)
add_test(testgamsset testgamsset)
