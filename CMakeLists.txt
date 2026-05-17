# oldstable as of May '26.
# Plus, I'm pretty sure it's required for things like CONFIGURE_DEPENDS.
cmake_minimum_required(VERSION 3.25)

project(privilege_check CXX)

if(MSVC)
    add_compile_options(/MP /W4)
else()
    add_compile_options(-Wall -Wextra)
endif()

add_compile_definitions(WIN32_LEAN_AND_MEAN NOMINMAX)

file(GLOB cpp_files CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB header_files CONFIGURE_DEPENDS "src/*.h" "src/*.hpp")
file(GLOB rc_files CONFIGURE_DEPENDS "src/*.rc")
add_executable(privilege_check WIN32 ${cpp_files} ${header_files} ${rc_files})

if(MSVC)
    target_compile_definitions(privilege_check PRIVATE _UNICODE UNICODE)
endif()

# MinGW-w64 is assumed when MINGW is triggered.
# Not sure if that matters a great deal.

if(MINGW)
    target_compile_options(privilege_check PRIVATE -municode)
    target_link_libraries(privilege_check PRIVATE -municode)
endif()

# NTDDI_VERSION & _WIN32_WINNT have to be defined in order to use the UAC
# features (available on Vista and later):
# https://msdn.microsoft.com/en-us/library/aa383745.aspx
if(MINGW)
    target_compile_definitions(privilege_check PRIVATE
        NTDDI_VERSION=NTDDI_VISTA
        _WIN32_WINNT=_WIN32_WINNT_VISTA)
endif()

install(TARGETS privilege_check RUNTIME DESTINATION bin)
if(MSVC)
    install(FILES $<TARGET_PDB_FILE:privilege_check> DESTINATION bin OPTIONAL)
endif()

install(FILES README.md LICENSE.txt DESTINATION share)
