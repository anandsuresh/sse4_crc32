/**
 * @file crc32.c
 * @brief CRC32 calculator based on Intel's Streaming SIMD Extensions 4.2
 *
 * Provides a binding between Javascript and the C implementation of the CRC32
 * function.
 *
 * @author Anand Suresh <anandsuresh@gmail.com>
 */

#include <nan.h>


using namespace v8;
using namespace node;


extern "C" {
    uint32_t calculateCRC32C(uint32_t initial_crc, char *str, size_t len);
}


/**
 * Generates 32-bit CRC
 */
NAN_METHOD(calculate) {
    NanScope();
    uint32_t init_crc;
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
            NanThrowTypeError("Initial CRC32 must be an integer value!");
            NanReturnUndefined();
        }

        init_crc = args[1]->Uint32Value();
    } else {
        init_crc = 0;
    }

    // Ensure the argument is a buffer or a string
    if (Buffer::HasInstance(args[0])) {
        Local<Object> buf = args[0]->ToObject();
        crc = calculateCRC32C(init_crc, (char *) Buffer::Data(buf), (size_t) Buffer::Length(buf));
    } else if (args[0]->IsObject()) {
        NanThrowTypeError("Cannot compute CRC-32 for objects!");
        NanReturnUndefined();
    } else {
        Local<String> strInput = args[0]->ToString();
        crc = calculateCRC32C(init_crc, (char *) *String::Utf8Value(strInput), (size_t) strInput->Utf8Length());
    }

    // Calculate the 32-bit CRC
    NanReturnValue(NanNew<Uint32>(crc));
}


/**
 * Initialize the module
 */
void init(Handle<Object> target) {
    NODE_SET_METHOD(target, "calculate", calculate);
}


NODE_MODULE(sse4_crc32, init)
