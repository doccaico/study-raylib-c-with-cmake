#include <stdio.h>

#define T vec_int, int
#include "stc/vec.h"

int main(void)
{
    vec_int vec = {0};

    vec_int_push(&vec, 25);
    vec_int_push(&vec, 13);

    printf("- lifegame -\n");

    for (int i = 0; i < vec_int_size(&vec); ++i) {
        printf(" %d", vec.data[i]);
    }

    for (c_each(k, vec_int, vec)) {
        printf(" %d", *k.ref);
    }

    vec_int_drop(&vec);
}
