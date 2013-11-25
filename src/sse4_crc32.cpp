/**
 * @file crc32.c
 * @brief CRC32 calculator based on Intel's Streaming SIMD Extensions 4.2
 *
 * Provides a binding between Javascript and the C implementation of the CRC32 function.
 *
 * @author Anand Suresh <anandsuresh@gmail.com>
 */

#include <node.h>
#include <node_buffer.h>
#include <v8.h>

using namespace v8;
using namespace node;

extern "C" {
uint32_t calculateCRC32C(uint32_t initial_crc, char *str, size_t len);
}

/**
 * Generates 32-bit CRC
 */
Handle<Value> calculate(const Arguments& args) {
    HandleScope scope;
    uint32_t init_crc;
    char *str = NULL;
    size_t len = 0;

    // Ensure an argument is passed
    if (args.Length() < 1) {
        return scope.Close(Integer::New(0));
    } else if (args.Length() > 2) {
        ThrowException(Exception::TypeError(String::New("Invalid number of arguments!")));
        return scope.Close(Undefined());
    }

    // Ensure the argument is a buffer or a string
    if (Buffer::HasInstance(args[0])) {
        Local<Object> buf = args[0]->ToObject();
        str = Buffer::Data(buf);
        len = (size_t) Buffer::Length(buf);
    } else if (args[0]->IsObject()) {
        ThrowException(Exception::TypeError(String::New("Cannot compute CRC-32 for objects!")));
        return scope.Close(Undefined());
    } else {
        Local<String> strInput = args[0]->ToString();
        str = (char *) *(String::Utf8Value(strInput));
        len = (size_t) strInput->Utf8Length();
    }

    // Check for any initial CRC passed to the function
    if (args.Length() > 1) {
        if (!(args[1]->IsUint32())) {
            ThrowException(Exception::TypeError(String::New("Initial CRC32 must be an integer value!")));
            return scope.Close(Undefined());
        }

        init_crc = args[1]->Uint32Value();
    } else {
        init_crc = 0;
    }

    // Calculate the 32-bit CRC
    return scope.Close(Integer::NewFromUnsigned(calculateCRC32C(init_crc, str, len)));
}

/**
 * Initialize the module
 */
void init(Handle<Object> target) {
    target->Set(String::NewSymbol("calculate"), FunctionTemplate::New(calculate)->GetFunction());
}

NODE_MODULE(sse4_crc32, init)
