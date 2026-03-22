add_executable(ParallelPrimeSearch)


target_include_directories(ParallelPrimeSearch PRIVATE include)


target_sources(ParallelPrimeSearch PRIVATE
    src/main.cpp
    src/primes.cpp
)


target_link_libraries(ParallelPrimeSearch PRIVATE 
    Threads::Threads
)
