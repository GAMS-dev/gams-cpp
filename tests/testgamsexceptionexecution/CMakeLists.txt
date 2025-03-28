include(../include.cmake)
include_directories(. .. ../../src ${CMAKE_BINARY_DIR}/inc/)

set(SOURCE ${SOURCE}
  ../testgamsobject.cpp
  ../../src/gamspath.cpp
  testgamsexceptionexecution.cpp)

add_executable(testgamsexceptionexecution ${SOURCE})
target_link_libraries(testgamsexceptionexecution gtest gamscpp)
add_test(testgamsexceptionexecution testgamsexceptionexecution)
