cmake_minimum_required(VERSION 3.15)
project(ParallelPrimeSearch LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find threading library
find_package(Threads REQUIRED)

# Executable target including all your source files
add_executable(ParallelPrimeSearch 
    main.cpp
)

# Link threads
target_link_libraries(ParallelPrimeSearch PRIVATE Threads::Threads)