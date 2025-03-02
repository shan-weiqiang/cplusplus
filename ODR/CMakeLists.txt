cmake_minimum_required(VERSION 3.16)
project(app)
add_library(s STATIC s.cpp dummy.cpp)
target_compile_options(s PRIVATE -fPIC)
add_library(a SHARED a.cpp)
target_link_libraries(a PUBLIC s)
add_library(b SHARED b.cpp)

target_link_libraries(b PUBLIC s)

add_executable(app app.cpp)
# order or a and b influence the link results
target_link_libraries(app PRIVATE a b)
