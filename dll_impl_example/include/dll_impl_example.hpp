#pragma once

#if defined(_WIN32)
#define ALLOWS_EXPORT __declspec(dllexport)
#else
#define ALLOWS_EXPORT __attribute__((visibility("default")))
#endif

ALLOWS_EXPORT void Share();
void Keep();
