cmake_minimum_required(VERSION 3.17)

project(examples)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

add_subdirectory(transportGDX)
add_subdirectory(transport1)
add_subdirectory(transport2)
add_subdirectory(transport3)
add_subdirectory(transport4)
add_subdirectory(transport5)
add_subdirectory(transport6)
add_subdirectory(transport7)
add_subdirectory(transport8)
# The following two require Qt to be installed
# add_subdirectory(transport9)
# add_subdirectory(transport10)
add_subdirectory(transport11)
add_subdirectory(transport12)
add_subdirectory(transport13)
add_subdirectory(transport14)
add_subdirectory(domainchecking)
add_subdirectory(warehouse)
add_subdirectory(transportEngine)
# add_subdirectory(xp_associative)
# add_subdirectory(xp_associative_vec)
# add_subdirectory(xp_dataWrite)
# add_subdirectory(xp_example1)
# add_subdirectory(xp_example2)
