#include <node.h>
#include <v8.h>
#include <stdio.h>

using namespace v8;

extern "C" {
uint32_t generateCRC32C(char *str, int len);
}

/**
 * Generates 32-bit CRC
 */
Handle<Value> GenerateCRC32(const Arguments& args) {
    HandleScope scope;
    Local<String> input;
    Local<Integer> output;

    // Ensure an argument is passed
    if (args.Length() < 1) {
        return scope.Close(Integer::New(0));
    }

    // Ensure the argument is not an object
    if (args[0]->IsObject()) {
        ThrowException(Exception::TypeError(String::New("Cannot compute CRC-32 for objects!")));
        return scope.Close(Undefined());
    }

    // Convert the argument into a string
    input = args[0]->ToString();

    // Calculate the 32-bit CRC
    output = Integer::NewFromUnsigned(generateCRC32C((char *) *(v8::String::Utf8Value(input)), input->Utf8Length()));
    return scope.Close(output);
}

/**
 * Initialize the module
 */
void init(Handle<Object> target) {
    target->Set(String::NewSymbol("generate"), FunctionTemplate::New(GenerateCRC32)->GetFunction());
}

NODE_MODULE(sse4_crc32, init)
