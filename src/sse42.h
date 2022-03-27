/**
 * @file sse42.h
 * @brief CRC32 calculator based on Intel's Streaming SIMD Extensions 4.2
 *
 * @author Anand Suresh <anandsuresh@gmail.com>
 * @copyright Copyright (C) 2018-present Anand Suresh. All rights reserved.
 */

#ifndef __CRC32C_SSE42__
#define __CRC32C_SSE42__

#include <stddef.h>
#include <stdint.h>

bool sse42_is_available();
void sse42_init();
#if defined(_MSC_VER) || defined(__x86_64__) || defined(__i386__)
// Skip as sse42.c is not compiled for other architectures.
uint32_t sse42_calculate(uint32_t, const char *, size_t);
#endif

#endif // __CRC32C_SSE42__
