include(../include.cmake)
include_directories(. .. ../../src ${CMAKE_BINARY_DIR}/inc/)

set(SOURCE ${SOURCE}
  ../testgamsobject.cpp
  ../../src/gamspath.cpp
  testgamsdatabaseiter.cpp)

add_executable(testgamsdatabaseiter ${SOURCE})
target_link_libraries(testgamsdatabaseiter gtest gamscpp)
add_test(testgamsdatabaseiter testgamsdatabaseiter)
