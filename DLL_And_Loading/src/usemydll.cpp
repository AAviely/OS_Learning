#include "pch.h"

extern "C" __declspec(dllimport) void Share();
// Other option would be-
// #include "mydll.h"

int main() {
    Share();
    return 0;
}