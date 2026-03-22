add_executable(dll_usage_example)

target_include_directories(dll_usage_example PRIVATE include)


target_sources(dll_usage_example PRIVATE
    src/dll_usage_example.cpp
)

target_link_libraries(dll_usage_example PRIVATE
    dll_impl_example
)
