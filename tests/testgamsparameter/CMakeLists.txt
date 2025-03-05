include(../include.cmake)
include_directories(. .. ../../src ${CMAKE_BINARY_DIR}/inc/)

set(SOURCE ${SOURCE}
  ../testgamsobject.cpp
  ../../src/gamspath.cpp
  testgamsparameter.cpp)

add_executable(testgamsparameter ${SOURCE})
target_link_libraries(testgamsparameter gtest gamscpp)
add_test(testgamsparameter testgamsparameter)
