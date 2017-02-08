# Copyright (c) 2016 Egor Tensin <Egor.Tensin@gmail.com>
# This file is part of the "Privilege check" project.
# For details, see https://github.com/egor-tensin/privilege-check.
# Distributed under the MIT License.

project(privilege_check CXX)

# MinGW-w64 is assumed when MINGW is triggered.
# Not sure if that matters a great deal.

get_directory_property(parent_directory PARENT_DIRECTORY)
set(is_root_project $<NOT:parent_directory>)

set(PRIVILEGE_CHECK_USE_STATIC_RUNTIME "${is_root_project}" CACHE BOOL "Link the runtime statically")
set(PRIVILEGE_CHECK_STRIP_SYMBOL_TABLE "${is_root_project}" CACHE BOOL "Strip symbol tables")

if(MSVC)
    add_compile_options(/MP /W4)
elseif(CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-Wall -Wextra)
endif()

function(use_static_runtime_msvc target)
    target_compile_options("${target}" PRIVATE
        $<$<CONFIG:Debug>:/MTd>
        $<$<NOT:$<CONFIG:Debug>>:/MT>)
endfunction()

function(use_static_runtime_gcc target)
    get_target_property(target_type "${target}" TYPE)
    if(target_type STREQUAL EXECUTABLE)
        target_link_libraries("${target}" PRIVATE -static)
    endif()
endfunction()

function(use_static_runtime target)
    if(MSVC)
        use_static_runtime_msvc("${target}")
    elseif(CMAKE_COMPILER_IS_GNUCXX)
        use_static_runtime_gcc("${target}")
    else()
        message(WARNING "Unrecognized toolset")
    endif()
endfunction()

function(strip_symbol_table_gcc target)
    target_link_libraries("${target}" PRIVATE
        $<$<OR:$<CONFIG:Release>,$<CONFIG:MinSizeRel>>:-s>)
endfunction()

function(strip_symbol_table target)
    if(MSVC)
    elseif(CMAKE_COMPILER_IS_GNUCXX)
        strip_symbol_table_gcc("${target}")
    else()
        message(WARNING "Unrecognized toolset")
    endif()
endfunction()

macro(add_executable target)
    _add_executable(${ARGV})
    if(TARGET "${target}")
        if(PRIVILEGE_CHECK_USE_STATIC_RUNTIME)
            use_static_runtime("${target}")
        endif()
        if(PRIVILEGE_CHECK_STRIP_SYMBOL_TABLE)
            strip_symbol_table("${target}")
        endif()
    endif()
endmacro()

file(GLOB source_files "src/*.cpp")
file(GLOB header_files "src/*.h" "src/*.hpp")
file(GLOB resource_files "src/*.rc")
add_executable(privilege_check WIN32
    ${source_files}
    ${header_files}
    ${resource_files})

if(CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(privilege_check PRIVATE -std=c++11)
endif()

if(MSVC)
    target_compile_definitions(privilege_check PRIVATE _UNICODE UNICODE)
endif()

if(MINGW)
    target_compile_options(privilege_check PRIVATE -municode)
    target_link_libraries(privilege_check PRIVATE -municode)
endif()

# NTDDI_VERSION & _WIN32_WINNT have to be defined in order to use UAC features
# (available on Vista and later):
# https://msdn.microsoft.com/en-us/library/aa383745.aspx
if(MINGW)
    target_compile_definitions(privilege_check PRIVATE
        NTDDI_VERSION=NTDDI_VISTA
        _WIN32_WINNT=_WIN32_WINNT_VISTA)
endif()
