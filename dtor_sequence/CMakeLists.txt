cmake_minimum_required(VERSION 3.16)
project(app)

add_library(dtor SHARED dtor.cpp)
add_executable(app someclass.cpp)
target_link_libraries(app dtor)