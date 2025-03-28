include(../include.cmake)
include_directories(. .. ../../src ${CMAKE_BINARY_DIR}/inc/)

set(SOURCE ${SOURCE}
  ../testgamsobject.cpp
  ../../src/gamspath.cpp
  testgamsexception.cpp)

add_executable(testgamsexception ${SOURCE})
target_link_libraries(testgamsexception gtest gamscpp)
add_test(testgamsexception testgamsexception)
