include(../include.cmake)
include_directories(. .. ../../src ${CMAKE_BINARY_DIR}/inc/)

set(SOURCE ${SOURCE}
  ../testgamsobject.cpp
  ../../src/gamspath.cpp
  testgamsparameterrecord.cpp)

add_executable(testgamsparameterrecord ${SOURCE})
target_link_libraries(testgamsparameterrecord gtest gamscpp)
add_test(testgamsparameterrecord testgamsparameterrecord)
