cmake_minimum_required(VERSION 2.8)

project(VTK-TUTORIALS CXX)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})


add_executable(First src/main.cpp)
target_link_libraries(First ${VTK_LIBRARIES})
