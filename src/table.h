/**
 * @file table.h
 * @brief CRC32C calculator based on table-lookups
 *
 * @author Anand Suresh <anandsuresh@gmail.com>
 * @copyright Copyright (C) 2018-present Anand Suresh. All rights reserved.
 */

#ifndef __CRC32C_TABLE__
#define __CRC32C_TABLE__

#include <stddef.h>
#include <stdint.h>

bool table_is_available();
void table_init();
uint32_t table_calculate(uint32_t, const char *, size_t);

#endif // __CRC32C_TABLE__
