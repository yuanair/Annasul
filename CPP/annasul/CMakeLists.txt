
file(GLOB_RECURSE SOURCES src/*.cpp src/*.hpp)

add_library(annasul STATIC ${SOURCES})
target_include_directories(annasul PUBLIC include)
