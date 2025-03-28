include(../include.cmake)

include_directories(. .. ../../src ${CMAKE_BINARY_DIR}/inc/)

set(SOURCE ${SOURCE}
  ../testgamsobject.cpp
  ../../src/gamspath.cpp
  testgamsdatabase.cpp)

add_executable(testgamsdatabase ${SOURCE})
target_link_libraries(testgamsdatabase gtest gamscpp)
add_test(testgamsdatabase testgamsdatabase)
