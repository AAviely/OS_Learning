cmake_minimum_required(VERSION 3.15)
project(proj LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# Find threading library
find_package(Threads REQUIRED)


# Targets
add_subdirectory(ParallelPrimeSearch)
#add_subdirectory(shared_mem)
add_subdirectory(dll_impl_example)
add_subdirectory(dll_usage_example)

# Remove any file(COPY_FILE) or file(GENERATE) lines related to compile_commands.json

# This creates a target that runs AFTER the configuration is successful
add_custom_target(copy_compile_commands ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_BINARY_DIR}/compile_commands.json"
        "${CMAKE_SOURCE_DIR}/compile_commands.json"
    DEPENDS "${CMAKE_BINARY_DIR}/compile_commands.json"
    COMMENT "Copying compile_commands.json to source root for IntelliSense"
)