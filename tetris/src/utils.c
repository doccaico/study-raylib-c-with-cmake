#include <stdio.h>
#include <stdlib.h>

#include "utils.h"


void* malloc_or_die(size_t size)
{
    void *ptr = malloc(size);

    if (ptr == NULL) {
        fputs("could not allocate memory\n", stderr);
        exit(1);
    }

    return ptr;
}
