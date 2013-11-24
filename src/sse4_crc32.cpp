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
    uint32_t generateCRC32C(char *str, size_t len);
}

/**
 * Generates 32-bit CRC
 */
Handle<Value> calculateCRC32(const Arguments& args) {
    HandleScope scope;
    Local<Integer> output;

    // Ensure an argument is passed
    if (args.Length() < 1) {
        return scope.Close(Integer::New(0));
    }

    // Ensure the argument is a buffer or a string
    if (Buffer::HasInstance(args[0])) {
        Local<Object> input = args[0]->ToObject();
        output = Integer::NewFromUnsigned(generateCRC32C(Buffer::Data(input), Buffer::Length(input)));
    } else if (args[0]->IsObject()) {
        ThrowException(Exception::TypeError(String::New("Cannot compute CRC-32 for objects!")));
        return scope.Close(Undefined());
    } else {
        Local<String> strInput = args[0]->ToString();
        output = Integer::NewFromUnsigned(
                generateCRC32C((char *) *(String::Utf8Value(strInput)), strInput->Utf8Length()));
    }

    // Calculate the 32-bit CRC
    return scope.Close(output);
}

/**
 * Initialize the module
 */
void init(Handle<Object> target) {
    target->Set(String::NewSymbol("calculateCRC32"), FunctionTemplate::New(calculateCRC32)->GetFunction());
}

NODE_MODULE(sse4_crc32, init)
