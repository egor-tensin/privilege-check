project(privilege_test)

file(GLOB ${PROJECT_NAME}_source_files "*.cpp")
file(GLOB ${PROJECT_NAME}_header_files "*.h")
file(GLOB ${PROJECT_NAME}_resource_files "*.rc")
add_executable(${PROJECT_NAME} WIN32
    ${${PROJECT_NAME}_source_files}
    ${${PROJECT_NAME}_header_files}
    ${${PROJECT_NAME}_resource_files})
target_compile_definitions(${PROJECT_NAME} PRIVATE _UNICODE)
