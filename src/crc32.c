/**
 * @file crc32.c
 * @brief CRC32 calculator based on Intel's Streaming SIMD Extensions 4.2
 *
 * Based on the CRC32C (Castagnoli)implementation, this code utilizes Intel's Streaming SIMD Extensions
 * instruction set, specifically the CRC32 instruction to calculate the CRC of any input string.
 *
 * @author Anand Suresh <anandsuresh@gmail.com>
 */

#include <stdint.h>
#include <smmintrin.h>

// Determine the architecture */
#ifdef __x86_64__
#  define calcCRC32C _mm_crc32_u64
#  define sysint_t uint64_t
#  define ALIGN 0xFUL                 // Align to 16 bytes
#else
#  define calcCRC32C _mm_crc32_u32
#  define sysint_t uint32_t
#  define ALIGN 0x4UL                 // Align to 4 bytes
#endif

#define ALIGNMENT_MASK (ALIGN - 1)
#define WORD_SIZE sizeof (void *)

// Debug support
#ifdef DEBUG
# include <stdio.h>
# define log(...)              printf(__VA_ARGS__)
# define log_op(len, ptr, crc) log("Speed: %lu bytes\tptr: %p\tcrc: 0x%9x\tstr: %s\n", (size_t)len, ptr, crc, ptr)
#else
# define log(...)
# define log_op(len, ptr, crc)
#endif

/**
 * Calculates 32-bit CRC for the specified input string
 *
 * @return A 32-bit unsigned integer representing the CRC
 */
uint32_t calculateCRC32C(uint32_t initial_crc, char *str, size_t len) {
    // If the string is empty, return 0
    if (len == 0) {
        return initial_crc;
    }

    // Align the input to the word boundary
    for (; (len > 0) && ((sysint_t) str & ALIGNMENT_MASK); len--, str++) {
        initial_crc = _mm_crc32_u8(initial_crc, *str);
        log_op(1, str, initial_crc);
    }

    // Blast off the CRC32 calculation using the system word size
    for (; len >= WORD_SIZE; len -= WORD_SIZE, str += WORD_SIZE) {
        initial_crc = calcCRC32C(initial_crc, *(sysint_t *) str);
        log_op(WORD_SIZE, str, initial_crc);
    }

    // Calculate the CRC for any remaining portions of the string
    for (; len >= 4; len -= 4, str += 4) {                           // Only used for 64-bit systems
        initial_crc = _mm_crc32_u32(initial_crc, *(uint32_t *) str);
        log_op(4, str, initial_crc);
    }

    for (; len >= 2; len -= 2, str += 2) {
        initial_crc = _mm_crc32_u16(initial_crc, *(uint16_t *) str);
        log_op(2, str, initial_crc);
    }

    if (len > 0) {
        initial_crc = _mm_crc32_u8(initial_crc, *str);
        log_op(1, str, initial_crc);
    }

    return initial_crc;
}
