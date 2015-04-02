/**
 * @file crc32.c
 * @brief CRC32 calculator based on Intel's Streaming SIMD Extensions 4.2
 *
 * The code below provides the bindings for the node-addon allowing for interfacing of C/C++ code with
 * JavaScript. It implemements 2 versions of the CRC-32C calculator:
 * - The hardware-accelerated version that uses Intel's SSE 4.2 instructions
 * - A table-lookup based CRC calculated implemented in software for non-Nehalam-based architectures
 *
 * NOTES:
 * - This code currently works on little-endian hardware only!
 * - Table-based CRC-32C implementation based on code by Mark Adler at http://stackoverflow.com/a/17646775.
 *
 * @author Anand Suresh <anandsuresh@gmail.com>
 */

#include <stdint.h>
#include <nan.h>
#include <smmintrin.h>



using namespace v8;
using namespace node;



// Bit-mask for the SSE 4.2 flag in the CPU ID
#define SSE4_2_FLAG         0x100000

// The CRC-32C polynomial in reversed bit order
#define CRC32C_POLYNOMIAL   0x82f63b78

// Byte-boundary alignment issues
#define ALIGN_SIZE          0x08UL              // Align at an 8-byte boundary
#define ALIGN_MASK          (ALIGN_SIZE - 1)    // Bitmask for 8-byte bound addresses

// Performs H/W CRC operations
#define CALC_CRC(op, crc, type, buf, len)                                                                    \
    do {                                                                                                     \
        for (; (len) >= sizeof(type); (len) -= sizeof(type), buf += sizeof(type)) {                          \
            (crc) = op((crc), *(type *) (buf));                                                              \
        }                                                                                                    \
    } while(0)



// Stores the CRC-32 lookup table for the software-fallback implementation
static uint32_t crc32cTable[8][256];



void cpuid(uint32_t op, uint32_t reg[4]) {
#if defined(__x86_64__)
    __asm__ volatile(
        "pushq %%rbx       \n\t"
        "cpuid             \n\t"
        "movl  %%ebx, %1   \n\t"
        "popq  %%rbx       \n\t"
        : "=a"(reg[0]), "=r"(reg[1]), "=c"(reg[2]), "=d"(reg[3])
        : "a"(op)
        : "cc");
#else
    __asm__ volatile(
        "pushl %%ebx       \n\t"
        "cpuid             \n\t"
        "movl  %%ebx, %1   \n\t"
        "popl  %%ebx       \n\t"
        : "=a"(reg[0]), "=r"(reg[1]), "=c"(reg[2]), "=d"(reg[3])
        : "a"(op)
        : "cc");
#endif
}


/**
 * Returns whether or not Intel's Streaming SIMD Extensions 4.2 is available on the hardware
 *
 * @return true if Intel's Streaming SIMD Extensions 4.2 are present; otherwise false
 */
bool isSSE42Available() {
    uint32_t reg[4];

    cpuid(1, reg);
    return ((reg[2] >> 20) & 1) == 1;
}


/**
 * Initializes the CRC-32C lookup table for software-based CRC calculation
 */
void initCrcTable() {
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
        crc32cTable[0][i] = crc;
    }

    for (i = 0; i < 256; i++) {
        crc = crc32cTable[0][i];
        for (j = 1; j < 8; j++) {
            crc = crc32cTable[0][crc & 0xff] ^ (crc >> 8);
            crc32cTable[j][i] = crc;
        }
    }
}


/**
 * Calculates CRC-32C using the lookup table
 *
 * @param initialCrc The initial CRC to use for the operation
 * @param buf The buffer that stores the data whose CRC is to be calculated
 * @param len The size of the buffer
 * @return The CRC-32C of the data in the buffer
 */
uint32_t swCrc32c(uint32_t initialCrc, const char *buf, size_t len) {
    const char *next = buf;
    uint64_t crc = initialCrc;


    // If the string is empty, return 0
    if (len == 0) return (uint32_t)crc;

    // XOR the initial CRC with INT_MAX
    crc ^= 0xFFFFFFFF;

    // Process byte-by-byte until aligned to 8-byte boundary
    while (len && ((uintptr_t) next & 7) != 0) {
        crc = crc32cTable[0][(crc ^ *next++) & 0xff] ^ (crc >> 8);
        len--;
    }

    // Process 8 bytes at a time
    while (len >= 8) {
        crc ^= *(uint64_t *) next;
        crc = crc32cTable[7][(crc >>  0) & 0xff] ^ crc32cTable[6][(crc >>  8) & 0xff]
            ^ crc32cTable[5][(crc >> 16) & 0xff] ^ crc32cTable[4][(crc >> 24) & 0xff]
            ^ crc32cTable[3][(crc >> 32) & 0xff] ^ crc32cTable[2][(crc >> 40) & 0xff]
            ^ crc32cTable[1][(crc >> 48) & 0xff] ^ crc32cTable[0][(crc >> 56)];
        next += 8;
        len -= 8;
    }

    // Process any remaining bytes
    while (len) {
        crc = crc32cTable[0][(crc ^ *next++) & 0xff] ^ (crc >> 8);
        len--;
    }

    // XOR again with INT_MAX
    return (uint32_t)(crc ^= 0xFFFFFFFF);
}


/**
 * Calculates CRC-32C using hardware support
 *
 * @param initialCrc The initial CRC to use for the operation
 * @param buf The buffer that stores the data whose CRC is to be calculated
 * @param len The size of the buffer
 * @return The CRC-32C of the data in the buffer
 */
uint32_t hwCrc32c(uint32_t initialCrc, const char *buf, size_t len) {
    uint32_t crc = initialCrc;

    // If the string is empty, return the initial crc
    if (len == 0) return crc;

    // XOR the initial CRC with INT_MAX
    crc ^= 0xFFFFFFFF;

    // Align the input to the word boundary
    for (; (len > 0) && ((size_t)buf & ALIGN_MASK); len--, buf++) {
        crc = _mm_crc32_u8(crc, *buf);
    }

    // Blast off the CRC32 calculation on hardware
#if defined(__x86_64__) || defined(_M_X64)
    CALC_CRC(_mm_crc32_u64, crc, uint64_t, buf, len);
#endif
    CALC_CRC(_mm_crc32_u32, crc, uint32_t, buf, len);
    CALC_CRC(_mm_crc32_u16, crc, uint16_t, buf, len);
    CALC_CRC(_mm_crc32_u8, crc, uint8_t, buf, len);

    // XOR again with INT_MAX
    return (crc ^= 0xFFFFFFFF);
}



/**
 * Returns whether or not hardware support is available for CRC calculation
 */
NAN_METHOD(isHardwareCrcSupported) {
    NanScope();
    NanReturnValue(NanNew<Boolean>(isSSE42Available()));
}


/**
 * Calculates CRC-32C for the specified string/buffer using the lookup-table
 */
NAN_METHOD(swCrc) {
    NanScope();
    uint32_t initCrc;
    uint32_t crc;

    // Ensure an argument is passed
    if (args.Length() < 1) {
        NanReturnValue(NanNew<Integer>(0));
    } else if (args.Length() > 2) {
        NanThrowTypeError("Invalid number of arguments!");
        NanReturnUndefined();
    }

    // Check for any initial CRC passed to the function
    if (args.Length() > 1) {
        if (!(args[1]->IsUint32())) {
            NanThrowTypeError("Initial CRC-32 is not an integer value as expected!");
            NanReturnUndefined();
        }

        initCrc = args[1]->Uint32Value();
    } else {
        initCrc = 0;
    }

    // Ensure the argument is a buffer or a string
    if (Buffer::HasInstance(args[0])) {
        Local<Object> buf = args[0]->ToObject();
        crc = swCrc32c(initCrc, (const char *)Buffer::Data(buf), (size_t)Buffer::Length(buf));
    } else if (args[0]->IsObject()) {
        NanThrowTypeError("Cannot compute CRC-32 for objects!");
        NanReturnUndefined();
    } else {
        Local<String> strInput = args[0]->ToString();
        crc = swCrc32c(initCrc, (const char *)(*String::Utf8Value(strInput)), (size_t)strInput->Utf8Length());
    }

    // Calculate the 32-bit CRC
    NanReturnValue(NanNew<Uint32>(crc));
}


/**
 * Calculates CRC-32C for the specified string/buffer using Intel's SSE 4.2 instructions
 */
NAN_METHOD(hwCrc) {
    NanScope();
    uint32_t initCrc;
    uint32_t crc;

    // Ensure an argument is passed
    if (args.Length() < 1) {
        NanReturnValue(NanNew<Integer>(0));
    } else if (args.Length() > 2) {
        NanThrowTypeError("Invalid number of arguments!");
        NanReturnUndefined();
    }

    // Check for any initial CRC passed to the function
    if (args.Length() > 1) {
        if (!(args[1]->IsUint32())) {
            NanThrowTypeError("Initial CRC-32C is not an integer value as expected!");
            NanReturnUndefined();
        }

        initCrc = args[1]->Uint32Value();
    } else {
        initCrc = 0;
    }

    // Ensure the argument is a buffer or a string
    if (node::Buffer::HasInstance(args[0])) {
        Local<Object> buf = args[0]->ToObject();
        crc = hwCrc32c(initCrc, (const char *)Buffer::Data(buf), (size_t)Buffer::Length(buf));
    } else if (args[0]->IsObject()) {
        NanThrowTypeError("Cannot compute CRC-32C for objects!");
        NanReturnUndefined();
    } else {
        Local<String> strInput = args[0]->ToString();
        crc = hwCrc32c(initCrc, (const char *)(*String::Utf8Value(strInput)), (size_t)strInput->Utf8Length());
    }

    // Calculate the 32-bit CRC
    NanReturnValue(NanNew<Uint32>(crc));
}



/**
 * Initialize the module
 */
void init(Handle<Object> exports) {
    initCrcTable();

    NODE_SET_METHOD(exports, "isHardwareCrcSupported", isHardwareCrcSupported);
    NODE_SET_METHOD(exports, "swCrc", swCrc);
    NODE_SET_METHOD(exports, "hwCrc", hwCrc);
}


NODE_MODULE(sse4_crc32, init)
