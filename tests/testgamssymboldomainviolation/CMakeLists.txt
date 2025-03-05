include(../include.cmake)
include_directories(. .. ../../src ${CMAKE_BINARY_DIR}/inc/)

set(SOURCE ${SOURCE}
  ../testgamsobject.cpp
  ../../src/gamspath.cpp
  testgamssymboldomainviolation.cpp)

add_executable(testgamssymboldomainviolation ${SOURCE})
target_link_libraries(testgamssymboldomainviolation gtest gamscpp)
add_test(testgamssymboldomainviolation testgamssymboldomainviolation)
