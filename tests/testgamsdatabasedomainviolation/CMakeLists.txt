include(../include.cmake)
include_directories(. .. ../../src ${CMAKE_BINARY_DIR}/inc/)

set(SOURCE ${SOURCE}
  ../testgamsobject.cpp
  ../../src/gamspath.cpp
  testgamsdatabasedomainviolation.cpp)

add_executable(testgamsdatabasedomainviolation ${SOURCE})
target_link_libraries(testgamsdatabasedomainviolation gtest gamscpp)
add_test(testgamsdatabasedomainviolation testgamsdatabasedomainviolation)
