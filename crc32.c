#include <stdint.h>
#include <smmintrin.h>

uint32_t fastcrc(char *str, uint32_t len) {
    uint32_t *ptr = (void *) str;
    uint32_t i, crc = 0;

    for (i = 0; i < len; i = i + sizeof (uint32_t *))
        crc = _mm_crc32_u32(crc, *ptr++);

    return crc;
}
