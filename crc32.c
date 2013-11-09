/**
 * @file crc32.c
 *
 * @author Anand Suresh <anandsuresh@gmail.com>
 */

#include <stdint.h>
#include <smmintrin.h>


#include <stdio.h>

// Determine the word size for alignment
#define WORD_SIZE sizeof (void *)

// Determine the architecture */
#ifdef __x86_64__
#define calcCRC32C _mm_crc32_u64
#define sysint_t uint64_t
#define ALIGN 0xFUL                 // Align to 16 bytes   TODO Reconfirm this is correct!
#else
#define calcCRC32C _mm_crc32_u32
#define sysint_t uint32_t
#define ALIGN 0x4UL                 // Align to 4 bytes
#endif

#define ALIGNMENT_MASK (ALIGN - 1)

/**
 * Generates 32-bit CRC for the specified input string
 *
 * @return A 32-bit unsigned integer representing the CRC
 */
uint32_t generateCRC32C(char *str, int len) {
    sysint_t crc = 0;

    // If the string is empty, return 0
    if (len < 0) {
        return crc;
    }

    // Align the input to the word boundary
    for (; (len > 0) && ((sysint_t) str & ALIGNMENT_MASK); len--) {
        crc = _mm_crc32_u8(crc, *str++);
    }

    // Blast off the CRC32 calculation using the system word size
    for (; len >= WORD_SIZE; len -= WORD_SIZE) {
        crc = calcCRC32C(crc, (*(sysint_t *) str)++);
    }

    // Calculate the CRC for any remaining portions of the string
    if (len >= 2) {
        crc = _mm_crc32_u16(crc, (*(uint16_t *) str)++);
        len -= 2;
    }

    if (len > 0) {
        crc = _mm_crc32_u8(crc, *str++);
    }

    return crc;
}
