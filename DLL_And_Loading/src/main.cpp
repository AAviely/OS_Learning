#include <stdio.h>

#include "my_funcs.h"

int main() {
    static int a = 2;
    static int b = 3;
    int c;

    c = my_add(a, b);
    printf("%d", c);
    return 0;
}