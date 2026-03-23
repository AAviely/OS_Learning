add_library(dll_impl_example SHARED)

target_include_directories(dll_impl_example PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

set_target_properties(dll_impl_example PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
)

target_sources(dll_impl_example PRIVATE
    src/dll_impl_example.cpp
)
