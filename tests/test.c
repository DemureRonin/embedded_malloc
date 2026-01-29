#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "../src/embedded_malloc.h"
#include "structs.h"

static void check_memory_state(const char* test_name) {
    printf("[%s] Small bitmap: 0x%08X, Large bitmap: 0x%08X\n",
           test_name, bitmap_small, bitmap_large);
}

int main(void) {
    printf("TEST: Allocate all small blocks\n");
    struct small* small_blocks[BLOCK_SMALL_COUNT];
    for (int i = 0; i < BLOCK_SMALL_COUNT; i++) {
        small_blocks[i] = (struct small*)embedded_malloc(BLOCK_SMALL_SIZE);
        assert(small_blocks[i] != NULL);
        memset(small_blocks[i]->data, i, BLOCK_SMALL_SIZE);
    }
    check_memory_state("All small allocated");
    printf("All %d small blocks allocated successfully\n\n", BLOCK_SMALL_COUNT);

    printf("TEST: Overflow small blocks\n");
    struct small* extra = (struct small*)embedded_malloc(BLOCK_SMALL_SIZE);
    assert(extra == NULL);
    printf("Overflow correctly returns NULL\n\n");

    printf("TEST: Free specific small blocks\n");
    struct small* freed_block_0 = small_blocks[0];
    struct small* freed_block_5 = small_blocks[5];

    embedded_free(small_blocks[0]);
    embedded_free(small_blocks[5]);
    small_blocks[0] = NULL;
    small_blocks[5] = NULL;

    check_memory_state("Freed blocks 0 and 5");
    printf("Freed blocks 0 and 5 successfully\n\n");

    printf("TEST: Reallocate freed small blocks\n");
    struct small* realloc1 = (struct small*)embedded_malloc(BLOCK_SMALL_SIZE);
    struct small* realloc2 = (struct small*)embedded_malloc(BLOCK_SMALL_SIZE);
    assert(realloc1 != NULL && realloc2 != NULL);

    small_blocks[0] = realloc1;
    small_blocks[5] = realloc2;
    printf("Reallocated freed blocks successfully\n");
    printf("realloc1 = %p (was %p)\n", realloc1, freed_block_0);
    printf("realloc2 = %p (was %p)\n\n", realloc2, freed_block_5);

    printf("TEST: Allocate all large blocks\n");
    struct large* large_blocks[BLOCK_LARGE_COUNT];
    for (int i = 0; i < BLOCK_LARGE_COUNT; i++) {
        large_blocks[i] = (struct large*)embedded_malloc(BLOCK_LARGE_SIZE);
        assert(large_blocks[i] != NULL);
        memset(large_blocks[i]->data, i + 100, BLOCK_LARGE_SIZE);
    }
    check_memory_state("All large allocated");
    printf("All %d large blocks allocated successfully\n\n", BLOCK_LARGE_COUNT);

    printf("TEST: Overflow large blocks\n");
    struct large* extra_large = (struct large*)embedded_malloc(BLOCK_LARGE_SIZE);
    assert(extra_large == NULL);
    printf("Overflow correctly returns NULL\n\n");

    printf("TEST: Free all small blocks\n");
    for (int i = 0; i < BLOCK_SMALL_COUNT; i++) {
        if (small_blocks[i] != NULL) {
            embedded_free(small_blocks[i]);
            small_blocks[i] = NULL;
        }
    }
    check_memory_state("All small freed");
    printf("All small blocks freed successfully\n\n");

    printf("TEST: Free all large blocks\n");
    for (int i = 0; i < BLOCK_LARGE_COUNT; i++) {
        embedded_free(large_blocks[i]);
        large_blocks[i] = NULL;
    }
    check_memory_state("All large freed");
    printf("All large blocks freed successfully\n\n");

    printf("TEST: Final check - all memory should be free\n");
    assert(bitmap_small == 0);
    assert(bitmap_large == 0);

    struct small* final_small = (struct small*)embedded_malloc(BLOCK_SMALL_SIZE);
    struct large* final_large = (struct large*)embedded_malloc(BLOCK_LARGE_SIZE);
    assert(final_small != NULL);
    assert(final_large != NULL);

    embedded_free(final_small);
    embedded_free(final_large);

    check_memory_state("Final state");
    printf("TESTS COMPLETED SUCCESSFULLY\n");

    return 0;
}