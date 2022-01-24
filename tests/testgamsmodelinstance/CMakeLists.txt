include(../include.cmake)
include_directories(. .. ../../src ${CMAKE_BINARY_DIR}/inc/)

set(SOURCE ${SOURCE} ../testgamsobject.cpp
                     ../../src/gamspath.cpp
                     testgamsmodelinstance.cpp)

add_executable(testgamsmodelinstance ${SOURCE})
target_link_libraries(testgamsmodelinstance gtest gamscpp)
add_test(testgamsmodelinstance testgamsmodelinstance)
