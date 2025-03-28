include(../include.cmake)
include_directories(. .. ../../src ${CMAKE_BINARY_DIR}/inc/)

set(SOURCE ${SOURCE}
  ../testgamsobject.cpp
  ../../src/gamspath.cpp
  testgamsmodelinstanceopt.cpp)

add_executable(testgamsmodelinstanceopt ${SOURCE})
target_link_libraries(testgamsmodelinstanceopt gtest gamscpp)
add_test(testgamsmodelinstanceopt testgamsmodelinstanceopt)
