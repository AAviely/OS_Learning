add_library(SharedExpr)

# Find threading library
find_package(Threads REQUIRED)

# Executable target including all your source files
target_sources(SharedExpr PRIVATE
    mydll.cpp
    main.cpp
)

# Link threads
target_link_libraries(SharedExpr PRIVATE
    Threads::Threads
)
