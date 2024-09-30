

function(traverse_and_add_files dir)
    file(GLOB_RECURSE CPP_FILES "${dir}/*.cpp")
    file(GLOB_RECURSE HPP_FILES "${dir}/*.hpp" "${dir}/*.inl")
    #    foreach (CPP_FILE ${CPP_FILES})
    #        get_filename_component(CPP_FILE_NAME ${CPP_FILE} NAME_WE)
    #        message(STATUS "CPP_FILES: ${CPP_FILE}")
    #    endforeach ()
    #    foreach (HPP_FILE ${HPP_FILES})
    #        get_filename_component(HPP_FILE_NAME ${HPP_FILE} NAME_WE)
    #        message(STATUS "HPP_FILES: ${HPP_FILE}")
    #    endforeach ()
    set(sources ${sources} ${CPP_FILES} PARENT_SCOPE)
    set(headers ${headers} ${HPP_FILES} PARENT_SCOPE)
endfunction()

function(traverse_directories_and_add_files dir)
    message(STATUS "Subdirectory: ${dir}")
    file(GLOB SUBDIRS "${dir}/*")
    foreach (SUBDIR ${SUBDIRS})
        if (IS_DIRECTORY ${SUBDIR})
            get_filename_component(SUBDIR_NAME ${SUBDIR} NAME)
            if (SUBDIR_NAME STREQUAL "Windows")
                if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
                    traverse_and_add_files(${SUBDIR})
                    traverse_directories_and_add_files(${SUBDIR})
                endif ()
            elseif (SUBDIR_NAME STREQUAL "Linux")
                if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
                    traverse_and_add_files(${SUBDIR})
                    traverse_directories_and_add_files(${SUBDIR})
                endif ()
            elseif (SUBDIR_NAME STREQUAL "Mac")
                if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
                    traverse_and_add_files(${SUBDIR})
                    traverse_directories_and_add_files(${SUBDIR})
                endif ()
            elseif (SUBDIR_NAME STREQUAL "Android")
                if (CMAKE_SYSTEM_NAME STREQUAL "Android")
                    traverse_and_add_files(${SUBDIR})
                    traverse_directories_and_add_files(${SUBDIR})
                endif ()
            elseif (SUBDIR_NAME STREQUAL "Editor")
                if (ENABLE_FEATURE_EDITORS)
                    traverse_and_add_files(${SUBDIR})
                    traverse_directories_and_add_files(${SUBDIR})
                endif ()
            elseif (SUBDIR_NAME STREQUAL "Debug")
                if (ENABLE_FEATURE_DEBUG)
                    traverse_and_add_files(${SUBDIR})
                    traverse_directories_and_add_files(${SUBDIR})
                endif ()
            elseif (SUBDIR_NAME STREQUAL "Release")
                if (ENABLE_FEATURE_RELEASE)
                    traverse_and_add_files(${SUBDIR})
                    traverse_directories_and_add_files(${SUBDIR})
                endif ()
            else ()
                traverse_and_add_files(${SUBDIR})
                traverse_directories_and_add_files(${SUBDIR})
            endif ()
        endif ()
    endforeach ()
endfunction()


function(add_annasul_module path depend_libs)
    set(sources "")
    set(headers "")

    get_filename_component(SUBDIR_NAME ${path} NAME)

    if (depend_libs)
        message(STATUS "Module: <${SUBDIR_NAME}> Depend: \"${depend_libs}\"")
    else ()
        message(STATUS "Module: <${SUBDIR_NAME}>")
    endif ()

    traverse_and_add_files(${path}/Private)
    traverse_directories_and_add_files(${path}/Private)
    traverse_and_add_files(${path}/Public)
    traverse_directories_and_add_files(${path}/Public)

    add_library(Annasul${SUBDIR_NAME} STATIC ${sources} ${headers})
    target_include_directories(Annasul${SUBDIR_NAME} PUBLIC ${path}/Public PRIVATE ${path}/Private)

    if (depend_libs)
        target_link_libraries(Annasul${SUBDIR_NAME} PUBLIC ${depend_libs})
    endif ()
endfunction()

function(add_annasul_executable path depend_libs)
    set(sources "")
    set(headers "")

    get_filename_component(SUBDIR_NAME ${path} NAME)

    if (depend_libs)
        message(STATUS "Executable: <${SUBDIR_NAME}> Depend: \"${depend_libs}\"")
    else ()
        message(STATUS "Executable: <${SUBDIR_NAME}>")
    endif ()

    traverse_and_add_files(${path}/Private)
    traverse_directories_and_add_files(${path}/Private)
    traverse_and_add_files(${path}/Public)
    traverse_directories_and_add_files(${path}/Public)

    add_executable(Annasul${SUBDIR_NAME} ${sources} ${headers})
    target_include_directories(Annasul${SUBDIR_NAME} PUBLIC ${path}/Public PRIVATE ${path}/Private)

    if (depend_libs)
        target_link_libraries(Annasul${SUBDIR_NAME} PUBLIC ${depend_libs})
    endif ()
endfunction()


if (ENABLE_FEATURE_EDITORS)
    add_annasul_module("./Source/Editor/Editor" "AnnasulLaunch;AnnasulRender")
endif ()

# first layer
add_annasul_module("./Source/Runtime/Core" "")

# second layer
add_annasul_module("./Source/Runtime/Engine" "AnnasulCore")
add_annasul_module("./Source/Runtime/Math" "AnnasulCore")
add_annasul_module("./Source/Runtime/Reflect" "AnnasulCore")
add_annasul_module("./Source/Runtime/Socket" "AnnasulCore;ws2_32.lib")

# third layer
add_annasul_module("./Source/Runtime/Launch" "AnnasulEngine")
add_annasul_module("./Source/Runtime/Render" "AnnasulMath;Imm32.lib;dwmapi.lib;d2d1.lib")

# fourth layer
add_annasul_module("./Source/Runtime/MultiWindow" "AnnasulEngine;AnnasulRender")

# test
add_annasul_executable("./Source/Test/A" "")
