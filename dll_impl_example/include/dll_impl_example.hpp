#pragma once

#if defined(_WIN32)
#define ALLOWS_EXPORT __declspec(dllexport)
#else
#define ALLOWS_EXPORT __attribute__((visibility("default")))
#endif

void Keep();

ALLOWS_EXPORT
void Share();
