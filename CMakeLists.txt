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
    add_compile_options(-Wall -Wextra -std=c++14)
endif()

file(GLOB ${PROJECT_NAME}_source_files "src/*.cpp")
file(GLOB ${PROJECT_NAME}_header_files "src/*.h" "src/*.hpp")
file(GLOB ${PROJECT_NAME}_resource_files "src/*.rc")
add_executable(${PROJECT_NAME} WIN32
    ${${PROJECT_NAME}_source_files}
    ${${PROJECT_NAME}_header_files}
    ${${PROJECT_NAME}_resource_files})

if(MSVC)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _UNICODE UNICODE)
elseif(MINGW)
    target_compile_options(${PROJECT_NAME} PRIVATE -municode)
    target_link_libraries(${PROJECT_NAME} PRIVATE -municode)
    # NTDDI_VERSION & _WIN32_WINNT have to be defined in order to use UAC
    # features (available on Vista and later):
    # https://msdn.microsoft.com/en-us/library/aa383745.aspx
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        NTDDI_VERSION=NTDDI_VISTA
        _WIN32_WINNT=_WIN32_WINNT_VISTA)
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
    target_link_libraries(${PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:-s>)
endif()

# I prefer the CRT library to be linked statically, so that the binary can
# easily be used on other computers.
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:/MT>
        $<$<CONFIG:Debug>:/MTd>)
elseif(CMAKE_COMPILER_IS_GNUCXX)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        -static-libgcc
        -static-libstdc++)
endif()
