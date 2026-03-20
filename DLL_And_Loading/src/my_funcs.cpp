#include "my_funcs.h"

int my_add(int a, int b) {
    static int one = 1;
    return a + b + one;
}