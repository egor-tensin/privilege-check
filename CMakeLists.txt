# oldstable as of May '26.
# Plus, I'm pretty sure it's required for things like CONFIGURE_DEPENDS.
cmake_minimum_required(VERSION 3.25)

project(privilege_check CXX)

if(MSVC)
    add_compile_options(/MP /W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

add_compile_definitions(WIN32_LEAN_AND_MEAN NOMINMAX)

add_subdirectory(src)

install(FILES README.md LICENSE.txt DESTINATION share)
