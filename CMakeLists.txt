# Copyright (c) 2016 Egor Tensin <Egor.Tensin@gmail.com>
# This file is part of the "Privilege check" project.
# For details, see https://github.com/egor-tensin/privilege-check.
# Distributed under the MIT License.

project(privilege_check CXX)

# MinGW-w64 is assumed when MINGW is triggered.
# Not sure if that matters a great deal.

if(MSVC)
    add_compile_options(/MP /W4)
elseif(CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-Wall -Wextra)
endif()

get_directory_property(parent_directory PARENT_DIRECTORY)
set(is_root_project $<NOT:parent_directory>)

function(use_static_runtime_msvc target)
    target_compile_options(${target} PRIVATE
        $<$<CONFIG:Release>:/MT>
        $<$<CONFIG:Debug>:/MTd>)
endfunction()

function(use_static_runtime_gcc target)
    get_target_property(type ${target} TYPE)
    if(type STREQUAL EXECUTABLE)
        target_link_libraries(${target} PRIVATE -static)
    endif()
endfunction()

function(use_static_runtime target)
    if(MSVC)
        use_static_runtime_msvc(${target})
    elseif(CMAKE_COMPILER_IS_GNUCXX)
        use_static_runtime_gcc(${target})
    endif()
endfunction()

function(strip_symbol_table_gcc target)
    target_link_libraries(${target} PRIVATE $<$<CONFIG:Release>:-s>)
endfunction()

function(strip_symbol_table target)
    if(CMAKE_COMPILER_IS_GNUCXX)
        strip_symbol_table_gcc(${target})
    endif()
endfunction()

macro(add_executable target)
    _add_executable(${ARGV})
    if(TARGET ${target} AND is_root_project)
        use_static_runtime(${target})
        strip_symbol_table(${target})
    endif()
endmacro()

file(GLOB ${PROJECT_NAME}_source_files "src/*.cpp")
file(GLOB ${PROJECT_NAME}_header_files "src/*.h" "src/*.hpp")
file(GLOB ${PROJECT_NAME}_resource_files "src/*.rc")
add_executable(${PROJECT_NAME} WIN32
    ${${PROJECT_NAME}_source_files}
    ${${PROJECT_NAME}_header_files}
    ${${PROJECT_NAME}_resource_files})

if(CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(${PROJECT_NAME} PRIVATE -std=c++11)
endif()

if(MSVC)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _UNICODE UNICODE)
endif()

if(MINGW)
    target_compile_options(${PROJECT_NAME} PRIVATE -municode)
    target_link_libraries(${PROJECT_NAME} PRIVATE -municode)
endif()

# NTDDI_VERSION & _WIN32_WINNT have to be defined in order to use UAC features
# (available on Vista and later):
# https://msdn.microsoft.com/en-us/library/aa383745.aspx
if(MINGW)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        NTDDI_VERSION=NTDDI_VISTA
        _WIN32_WINNT=_WIN32_WINNT_VISTA)
endif()
