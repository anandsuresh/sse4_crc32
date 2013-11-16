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
# define log(len, ptr, crc) printf("Speed: %d bytes\tptr: %p\tcrc: 0x%9x\tstr: %s\n", len, ptr, crc, ptr)
#else
# define log(len, ptr, crc)
#endif

/**
 * Generates 32-bit CRC for the specified input string
 *
 * @return A 32-bit unsigned integer representing the CRC
 */
uint32_t generateCRC32C(char *str, int len) {
    uint32_t crc = 0;

    // If the string is empty, return 0
    if (len < 0) {
        return crc;
    }

    // Align the input to the word boundary
    for (; (len > 0) && ((sysint_t) str & ALIGNMENT_MASK); len--, str++) {
        crc = _mm_crc32_u8(crc, *str);
        log(1, str, crc);
    }

    // Blast off the CRC32 calculation using the system word size
    for (; len >= (int) WORD_SIZE; len -= (int) WORD_SIZE, str += (int) WORD_SIZE) {
        crc = calcCRC32C(crc, *(sysint_t *) str);
        log((int) WORD_SIZE, str, crc);
    }

    // Calculate the CRC for any remaining portions of the string
    for (; len >= 4; len -= 4, str += 4) {                           // Only used for 64-bit systems
        crc = _mm_crc32_u32(crc, *(uint32_t *) str);
        log(4, str, crc);
    }

    for (; len >= 2; len -= 2, str += 2) {
        crc = _mm_crc32_u16(crc, *(uint16_t *) str);
        log(2, str, crc);
    }

    if (len > 0) {
        crc = _mm_crc32_u8(crc, *str);
        log(1, str, crc);
    }

    return crc;
}
