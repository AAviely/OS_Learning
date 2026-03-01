cmake_minimum_required(VERSION 3.20)
project(MyCppProject VERSION 1.0 LANGUAGES CXX)

# Require modern C++ (e.g., C++20)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add your main executable
add_executable(SharedMemApp SharedMem_summary.cpp)