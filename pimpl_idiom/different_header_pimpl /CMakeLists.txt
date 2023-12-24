cmake_minimum_required(VERSION 3.16)
project(app)

add_library(some SHARED someclass.cpp)
add_executable(app app.cpp)
target_link_libraries(app some)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")