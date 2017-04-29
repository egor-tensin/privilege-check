project(privilege_check CXX)

include(common.cmake)

file(GLOB cpp_files "src/*.cpp")
file(GLOB header_files "src/*.h" "src/*.hpp")
file(GLOB rc_files "src/*.rc")
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
