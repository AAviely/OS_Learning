#include <stdio.h>

#include <dll_impl_example.hpp>

void Share() { printf("I am an exported function, can be called outside the DLL\n"); }

void Keep() { printf("I am not exported, can be called only within the DLL\n"); }

static bool Receptor()  // Reserved
{
    Keep();
    Share();
    return true;
}

static bool v_initialized(([]() -> bool { return Receptor(); }()));
