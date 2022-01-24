include(../include.cmake)
include_directories(. .. ../../src ${CMAKE_BINARY_DIR}/inc/)

set(SOURCE ${SOURCE} ../testgamsobject.cpp
                     ../../src/gamspath.cpp
                     testgamsworkspaceinfo.cpp)

add_executable(testgamsworkspaceinfo ${SOURCE})
target_link_libraries(testgamsworkspaceinfo gtest gamscpp)
add_test(testgamsworkspaceinfo testgamsworkspaceinfo)
