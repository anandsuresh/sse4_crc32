#include <stdint.h>

uint32_t hwCrc32c(uint32_t initialCrc, const char *buf, size_t len);

// The CRC-32C polynomial in reversed bit order
#define CRC32C_POLYNOMIAL   0x82f63b78
