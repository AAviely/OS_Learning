add_library(dll_impl_example)

target_include_directories(dll_impl_example PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_sources(dll_impl_example PRIVATE
    src/dll_impl_example.cpp
)
