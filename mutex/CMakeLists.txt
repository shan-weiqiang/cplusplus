cmake_minimum_required(VERSION 3.16)
project(app)
add_executable(app app.cpp)
target_link_libraries(app pthread)