cmake_minimum_required(VERSION 3.27)
project(Annasul)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-DUNICODE -D_UNICODE)

add_subdirectory(Annasul)

add_executable(Main main.cpp)
target_link_libraries(Main AnnasulCore AnnasulWindow)
