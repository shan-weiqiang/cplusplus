cmake_minimum_required(VERSION 3.0)

project(HelloWorld)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 14)

add_executable(app app.cpp)
target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/asio-1.30.2/include)
target_link_libraries(app PRIVATE pthread)