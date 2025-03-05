include(../include.cmake)
include_directories(. .. ../../src ${CMAKE_BINARY_DIR}/inc/)

set(SOURCE ${SOURCE}
  ../testgamsobject.cpp
  ../../src/gamspath.cpp
  testgamsvariablerecord.cpp)

add_executable(testgamsvariablerecord ${SOURCE})
target_link_libraries(testgamsvariablerecord gtest gamscpp)
add_test(testgamsvariablerecord testgamsvariablerecord)
