#include "embedded_malloc.h"

uint8_t pool_small[BLOCK_SMALL_COUNT][BLOCK_SMALL_SIZE] __attribute__((aligned(4)));
uint8_t pool_large[BLOCK_LARGE_COUNT][BLOCK_LARGE_SIZE] __attribute__((aligned(4)));

uint16_t bitmap_large = 0;
uint16_t bitmap_small = 0;

void *embedded_malloc(size_t size) {
    if (size == BLOCK_SMALL_SIZE) {
        if (bitmap_small >= BITMAP_SMALL_FULL) {
            return NULL;
        }
        int pos = __builtin_ctz(~bitmap_small);
        if (pos >= BLOCK_SMALL_COUNT) return NULL;
        bitmap_small |= (1 << pos);
        return &pool_small[pos];
    }
    if (size == BLOCK_LARGE_SIZE) {
        if (bitmap_large >= BITMAP_LARGE_FULL) {
            return NULL;
        }
        int pos = __builtin_ctz(~bitmap_large);
        if (pos >= BLOCK_LARGE_COUNT) return NULL;
        bitmap_large |= (1 << pos);
        return &pool_large[pos];
    }
    return NULL;
}

void embedded_free(void *ptr) {
    if (ptr == NULL) return;

    uint8_t *p = ptr;

    uint8_t *small_start = (uint8_t *) pool_small;
    uint8_t *small_end = small_start + BLOCK_SMALL_COUNT * BLOCK_SMALL_SIZE;
    if (p >= small_start && p < small_end) {
        uintptr_t offset = p - small_start;
        uint8_t index = offset / BLOCK_SMALL_SIZE;
        bitmap_small &= ~(1 << index);
        return;
    }

    uint8_t *large_start = (uint8_t *) pool_large;
    uint8_t *large_end = large_start + BLOCK_LARGE_COUNT * BLOCK_LARGE_SIZE;
    if (p >= large_start && p < large_end) {
        uintptr_t offset = p - large_start;
        uint8_t index = offset / BLOCK_LARGE_SIZE;
        bitmap_large &= ~(1U << index);
    }
}
