include(../include.cmake)
include_directories(. .. ../../src ${CMAKE_BINARY_DIR}/inc/)

set(SOURCE ${SOURCE}
  ../testgamsobject.cpp
  ../../src/gamspath.cpp
  testgamsjob.cpp)

add_executable(testgamsjob ${SOURCE})
target_link_libraries(testgamsjob gtest gamscpp)
add_test(testgamsjob testgamsjob)
