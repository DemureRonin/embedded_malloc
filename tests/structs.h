#ifndef EMBEDDED_MALLOC_STRUCTS_H
#define EMBEDDED_MALLOC_STRUCTS_H
#include <stdint.h>

struct small {
    uint8_t data[BLOCK_SMALL_SIZE];
};

struct large {
    uint8_t data[BLOCK_LARGE_SIZE];
};

#endif
