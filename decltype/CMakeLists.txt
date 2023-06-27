
cmake_minimum_required(VERSION 3.16)
project(app)

add_executable(app app.cpp)
target_compile_options(app PRIVATE -std=c++14)

