include(../include.cmake)
include_directories(. .. ../../src ${CMAKE_BINARY_DIR}/inc/)

set(SOURCE ${SOURCE}
  ../testgamsobject.cpp
  ../../src/gamspath.cpp
  testgamsvariable.cpp)

add_executable(testgamsvariable ${SOURCE})
target_link_libraries(testgamsvariable gtest gamscpp)
add_test(testgamsvariable testgamsvariable)
