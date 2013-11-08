#include <node.h>
#include <v8.h>
#include <stdio.h>

using namespace v8;

extern "C" {
void slowcrc_init();
uint32_t slowcrc(char *str, uint32_t len);
uint32_t fastcrc(char *str, uint32_t len);
}

/**
 * Generates 32-bit CRC
 */
Handle<Value> GenerateSlowCRC32(const Arguments& args) {
    HandleScope scope;
    Local<String> input;
    Local<Integer> output;

    // Ensure an argument is passed
    if (args.Length() < 1) {
        return scope.Close(Undefined());
    }

    // Ensure the argument is not an object
    if (args[0]->IsObject()) {
        ThrowException(Exception::TypeError(String::New("Cannot compute CRC-32 for objects!")));
        return scope.Close(Undefined());
    }

    // Convert the argument into a string
    input = args[0]->ToString();

    // Calculate the 32-bit CRC
    output = Integer::NewFromUnsigned(slowcrc((char *) *(v8::String::Utf8Value(input)), input->Utf8Length()));
    return scope.Close(output);
}

/**
 * Generates 32-bit CRC
 */
Handle<Value> GenerateFastCRC32(const Arguments& args) {
    HandleScope scope;
    Local<String> input;
    Local<Integer> output;

    // Ensure an argument is passed
    if (args.Length() < 1) {
        return scope.Close(Undefined());
    }

    // Ensure the argument is not an object
    if (args[0]->IsObject()) {
        ThrowException(Exception::TypeError(String::New("Cannot compute CRC-32 for objects!")));
        return scope.Close(Undefined());
    }

    // Convert the argument into a string
    input = args[0]->ToString();

    // Calculate the 32-bit CRC
    output = Integer::NewFromUnsigned(fastcrc((char *) *(v8::String::Utf8Value(input)), input->Utf8Length()));
    return scope.Close(output);
}

/**
 * Initialize the module
 */
void init(Handle<Object> target) {
    slowcrc_init();
    target->Set(String::NewSymbol("generateSlow"), FunctionTemplate::New(GenerateSlowCRC32)->GetFunction());
    target->Set(String::NewSymbol("generateFast"), FunctionTemplate::New(GenerateFastCRC32)->GetFunction());
}

NODE_MODULE(sse4_crc32, init)
