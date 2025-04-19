set(CMAKE_SYSTEM_NAME Android)
set(CMAKE_SYSTEM_VERSION 21)

if(DEFINED ENV{NDK_HOME})
    file(TO_CMAKE_PATH "$ENV{NDK_HOME}" ANDROID_NDK)
else()
    message(FATAL_ERROR "NDK_HOME environment variable not defined")
endif()

set(CMAKE_ANDROID_ARCH_ABI x86_64)
set(CMAKE_ANDROID_STL_TYPE c++_shared)

set(CMAKE_C_FLAGS "-Wall")
set(CMAKE_CPP_FLAGS "${CMAKE_C_FLAGS}")
