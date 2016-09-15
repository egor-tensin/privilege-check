project(privilege_test)

file(GLOB ${PROJECT_NAME}_source_files "src/*.cpp")
file(GLOB ${PROJECT_NAME}_header_files "src/*.h" "src/*.hpp")
file(GLOB ${PROJECT_NAME}_resource_files "src/*.rc")
add_executable(${PROJECT_NAME} WIN32
    ${${PROJECT_NAME}_source_files}
    ${${PROJECT_NAME}_header_files}
    ${${PROJECT_NAME}_resource_files})
target_compile_definitions(${PROJECT_NAME} PRIVATE _UNICODE)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
