include(../include.cmake)
include_directories(. .. ../../src ${CMAKE_BINARY_DIR}/inc/)

set(SOURCE ${SOURCE}
  ../testgamsobject.cpp
  ../../src/gamspath.cpp
  testgamsdomain.cpp)

add_executable(testgamsdomain ${SOURCE})
target_link_libraries(testgamsdomain gtest gamscpp)
add_test(testgamsdomain testgamsdomain)
