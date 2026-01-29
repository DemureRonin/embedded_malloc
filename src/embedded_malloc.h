/**
 * @file embedded_malloc.h
 * @brief Custom memory allocator for fixed-size blocks
 */

#ifndef EMBEDDED_ALLOC_H
#define EMBEDDED_ALLOC_H

#include <stdint.h>
#include <stddef.h>

/** @def BLOCK_SMALL_SIZE
 *  @brief Size of small memory blocks in bytes (15 bytes)
 */
#define BLOCK_SMALL_SIZE 15

/** @def BLOCK_LARGE_SIZE
 *  @brief Size of large memory blocks in bytes (180 bytes)
 */
#define BLOCK_LARGE_SIZE 180

/** @def BLOCK_SMALL_COUNT
 *  @brief Maximum number of small blocks available (16 blocks)
 */
#define BLOCK_SMALL_COUNT 16

/** @def BLOCK_LARGE_COUNT
 *  @brief Maximum number of large blocks available (4 blocks)
 */
#define BLOCK_LARGE_COUNT 4

/** @def BITMAP_SMALL_FULL
 *  @brief Bitmask representing all small blocks allocated (0xFFFF for 16 blocks)
 */
#define BITMAP_SMALL_FULL 0xFFFF

/** @def BITMAP_LARGE_FULL
 *  @brief Bitmask representing all large blocks allocated (0xF for 4 blocks)
 */
#define BITMAP_LARGE_FULL 0xF

/** @var pool_small
 *  @brief Memory pool for small blocks (16 × 15 bytes)
 */
extern uint8_t pool_small[BLOCK_SMALL_COUNT][BLOCK_SMALL_SIZE];

/** @var pool_large
 *  @brief Memory pool for large blocks (4 × 180 bytes)
 */
extern uint8_t pool_large[BLOCK_LARGE_COUNT][BLOCK_LARGE_SIZE];

/** @var bitmap_large
 *  @brief Bitmap tracking allocation status of large blocks (1 bit per block)
 */
extern uint16_t bitmap_large;

/** @var bitmap_small
 *  @brief Bitmap tracking allocation status of small blocks (1 bit per block)
 */
extern uint16_t bitmap_small;

/**
 * @brief Allocate a memory block of specified size
 * 
 * @param size Requested block size (must be 15 or 180 bytes)
 * @return void* Pointer to allocated memory, or NULL if no blocks available
 * 
 * @note Only two sizes are supported: 15 bytes (small) and 180 bytes (large)
 * @note Allocation is O(1) using bit operations
 */
void *embedded_malloc(size_t size);

/**
 * @brief Free a previously allocated memory block
 * 
 * @param ptr Pointer to memory block to free
 * 
 * @note If ptr is NULL, function does nothing
 * @note Function automatically determines which pool the pointer belongs to
 */
void embedded_free(void *ptr);

#endif