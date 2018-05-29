/**
 * @file table.cpp
 * @brief CRC32C calculator based on table-lookups
 *
 * Table-based CRC-32C implementation based on code from Mark Adler at
 * http://stackoverflow.com/a/17646775.
 *
 * @author Anand Suresh <anandsuresh@gmail.com>
 * @copyright Copyright (C) 2018-present Anand Suresh. All rights reserved.
 */

#include "table.h"

// The CRC-32C polynomial in reversed bit order
#define CRC32C_POLYNOMIAL 0x82f63b78

// Stores the CRC-32 lookup table for the software-fallback implementation
uint32_t table[8][256];

/**
 * Returns whether or not this mode is available
 *
 * NOTE: Table-based CRC-32C calculation is available on all platforms!
 *
 * @return true
 */
bool table_is_available() { return true; }

/**
 * Initializes the CRC-32C lookup table for software-based CRC calculation
 */
void table_init() {
  uint32_t i, j, crc;

  for (i = 0; i < 256; i++) {
    crc = i;
    crc = crc & 1 ? (crc >> 1) ^ CRC32C_POLYNOMIAL : crc >> 1;
    crc = crc & 1 ? (crc >> 1) ^ CRC32C_POLYNOMIAL : crc >> 1;
    crc = crc & 1 ? (crc >> 1) ^ CRC32C_POLYNOMIAL : crc >> 1;
    crc = crc & 1 ? (crc >> 1) ^ CRC32C_POLYNOMIAL : crc >> 1;
    crc = crc & 1 ? (crc >> 1) ^ CRC32C_POLYNOMIAL : crc >> 1;
    crc = crc & 1 ? (crc >> 1) ^ CRC32C_POLYNOMIAL : crc >> 1;
    crc = crc & 1 ? (crc >> 1) ^ CRC32C_POLYNOMIAL : crc >> 1;
    crc = crc & 1 ? (crc >> 1) ^ CRC32C_POLYNOMIAL : crc >> 1;
    table[0][i] = crc;
  }

  for (i = 0; i < 256; i++) {
    crc = table[0][i];
    for (j = 1; j < 8; j++) {
      crc = table[0][crc & 0xff] ^ (crc >> 8);
      table[j][i] = crc;
    }
  }
}

/**
 * Calculates CRC-32C using the lookup table
 *
 * @param initCrc The initial CRC to use for the operation
 * @param buf The buffer storing the data whose CRC is to be calculated
 * @param len The size of the buffer
 * @return The CRC-32C of the data in the buffer
 */
uint32_t table_calculate(uint32_t initCrc, const char *buf, size_t len) {
  const char *next = buf;
  uint64_t crc = initCrc;

  // If the string is empty, return 0
  if (len == 0) {
    return (uint32_t)crc;
  }

  // XOR the initial CRC with INT_MAX
  crc ^= 0xFFFFFFFF;

  // Process byte-by-byte until aligned to 8-byte boundary
  while (len && ((uintptr_t)next & 7) != 0) {
    crc = table[0][(crc ^ *next++) & 0xff] ^ (crc >> 8);
    len--;
  }

  // Process 8 bytes at a time
  while (len >= 8) {
    crc ^= *(uint64_t *)next;
    crc = table[7][(crc >> 0) & 0xff] ^
          table[6][(crc >> 8) & 0xff] ^
          table[5][(crc >> 16) & 0xff] ^
          table[4][(crc >> 24) & 0xff] ^
          table[3][(crc >> 32) & 0xff] ^
          table[2][(crc >> 40) & 0xff] ^
          table[1][(crc >> 48) & 0xff] ^
          table[0][(crc >> 56)];
    next += 8;
    len -= 8;
  }

  // Process any remaining bytes
  while (len) {
    crc = table[0][(crc ^ *next++) & 0xff] ^ (crc >> 8);
    len--;
  }

  // XOR again with INT_MAX
  return (uint32_t)(crc ^= 0xFFFFFFFF);
}
