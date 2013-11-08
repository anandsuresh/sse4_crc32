#include <stdint.h>

uint32_t slowcrc_table[1 << 8];


void slowcrc_init() {
    uint32_t i, j, a;

    for (i = 0; i < (1 << 8); i++) {
        a = ((uint32_t) i) << 24;
        for (j = 0; j < 8; j++) {
            if (a & 0x80000000)
                a = (a << 1) ^ 0x1EDC6F41;
            else
                a = (a << 1);
        }
        slowcrc_table[i] = a;
    }
}


uint32_t slowcrc(char *str, uint32_t len) {
    uint32_t lcrc = ~0;
    char *p, *e;

    e = str + len;
    for (p = str; p < e; ++p)
        lcrc = (lcrc >> 8) ^ slowcrc_table[(lcrc ^ (*p)) & 0xff];
    return ~lcrc;
}

uint32_t fastcrc(char *str, uint32_t len) {
    uint32_t q = len / sizeof(uint32_t),
        r = len % sizeof(uint32_t),
        *p = (uint32_t*) str, crc;

    crc = 0;
    while (q--) {
        __asm__ __volatile__(
            ".byte 0xf2, 0xf, 0x38, 0xf1, 0xf1;"
            :"=S" (crc)
            :"0" (crc), "c" (*p)
        );
        p++;
    }

    str = (char*) p;
    while (r--) {
        __asm__ __volatile__(
            ".byte 0xf2, 0xf, 0x38, 0xf0, 0xf1"
            :"=S" (crc)
            :"0" (crc), "c" (*str)
        );
        str++;
    }

    return crc;
}
