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

file(COPY_FILE
    "${CMAKE_BINARY_DIR}/compile_commands.json"
    "${CMAKE_SOURCE_DIR}/compile_commands.json"
)
