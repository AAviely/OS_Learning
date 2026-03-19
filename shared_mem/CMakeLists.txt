add_executable(SharedMemApp)


target_include_directories(SharedMemApp PRIVATE include)


# Add your main executable
target_sources(SharedMemApp PRIVATE
    src/SharedMem_summary.cpp
)
