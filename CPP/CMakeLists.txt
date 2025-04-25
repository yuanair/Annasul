cmake_minimum_required(VERSION 3.25)
project(hello_world VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF) 

add_executable(hello_module main.cppm)

add_executable(hello_world main.cpp)
 
add_subdirectory(annasul)
