include(../include.cmake)
include_directories(. .. ../../src ${CMAKE_BINARY_DIR}/inc/)

set(SOURCE ${SOURCE}
  ../testgamsobject.cpp
  ../../src/gamspath.cpp
  testgamsequationrecord.cpp)

add_executable(testgamsequationrecord ${SOURCE})
target_link_libraries(testgamsequationrecord gtest gamscpp)
add_test(testgamsequationrecord testgamsequationrecord)
