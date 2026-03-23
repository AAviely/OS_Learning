#include <stdio.h>

#include <cstdio>  // Use cstdio for safer startup printing
#include <dll_impl_example.hpp>

void Share() {
    // printf is safer during global initialization than std::cout
    printf("I am an exported function, can be called outside the DLL\n");
}

void Keep() { printf("I am not exported, can be called only within the DLL\n"); }

bool Receptor() {
    Share();
    Keep();
    return true;
}

// Fixed: Added 'return' keyword
auto a = []() -> bool { return Receptor(); }();