include(../include.cmake)

include_directories(. .. ../../src ${CMAKE_BINARY_DIR}/inc/)

set(SOURCE ${SOURCE}
  ../testgamsobject.cpp
  ../../src/gamspath.cpp
  testgamscheckpoint.cpp)

add_executable(testgamscheckpoint ${SOURCE})
target_link_libraries(testgamscheckpoint gtest gamscpp)
add_test(testgamscheckpoint testgamscheckpoint)
