/**
 * @file crc32c.cpp
 * @brief Node.js bindings for CRC-32C calculation
 *
 * The code below provides the bindings for the node-addon allowing for
 * interfacing of C/C++ code with JavaScript. It chooses between two versions of
 * the CRC-32C calculator:
 * - The hardware-accelerated version that uses Intel's SSE 4.2 instructions
 *   implemented in sse42.cpp
 * - A table-lookup based CRC calculated implemented in software for non-Nehalam
 *   architectures implemented in table.cpp
 *
 * NOTES:
 * - This code, though originally designed for little-endian hardware, should
 *   work for all platforms.
 * - Table-based CRC-32C implementation based on code by Mark Adler at
 *   http://stackoverflow.com/a/17646775.
 *
 * @author Anand Suresh <anandsuresh@gmail.com>
 * @copyright Copyright (C) 2018-present Anand Suresh. All rights reserved.
 */

#include "sse42.h"
#include "table.h"
#include <napi.h>
#include <stdio.h>

/**
 * Calculates CRC-32C for the specified string/buffer using SSE 4.2 extensions
 */
Napi::Value sse42_crc(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  uint32_t crc = 0;
  const char *buf;
  size_t len;

  // validate arguments
  if (info.Length() < 1) {
    return Napi::Number::New(env, 0);
  } else if (info.Length() > 2) {
    throw Napi::TypeError::New(env, "Invalid number of arguments!");
  }

  // use the specified initial CRC, if any
  if (info.Length() > 1) {
    if (!(info[1].IsNumber())) {
      throw Napi::TypeError::New(env, "Initial CRC-32C is not an integer!");
    }
    crc = info[1].As<Napi::Number>().Uint32Value();
  }

  // Ensure the argument is a buffer or a string
  if (info[0].IsBuffer()) {
    Napi::Buffer<char> data = info[0].As<Napi::Buffer<char>>();
    buf = (const char *)data.Data();
    len = (size_t)data.Length();
    return Napi::Number::New(env, sse42_calculate(crc, buf, len));
  } else if (info[0].IsString()) {
    std::string strInput = info[0].As<Napi::String>().Utf8Value();
    buf = (const char *)strInput.c_str();
    len = (size_t)strInput.length();
    return Napi::Number::New(env, sse42_calculate(crc, buf, len));
  } else {
    throw Napi::TypeError::New(env, "input is not a string/buffer!");
  }
}

/**
 * Calculates CRC-32C for the specified string/buffer using table-lookups
 */
Napi::Value table_crc(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  uint32_t crc = 0;
  const char *buf;
  size_t len;

  // validate arguments
  if (info.Length() < 1) {
    return Napi::Number::New(env, 0);
  } else if (info.Length() > 2) {
    throw Napi::TypeError::New(env, "Invalid number of arguments!");
  }

  // use the specified initial CRC, if any
  if (info.Length() > 1) {
    if (!(info[1].IsNumber())) {
      throw Napi::TypeError::New(env, "Initial CRC-32C is not an integer!");
    }
    crc = info[1].As<Napi::Number>().Uint32Value();
  }

  // Ensure the argument is a buffer or a string
  if (info[0].IsBuffer()) {
    Napi::Buffer<char> data = info[0].As<Napi::Buffer<char>>();
    buf = (const char *)data.Data();
    len = (size_t)data.Length();
    return Napi::Number::New(env, table_calculate(crc, buf, len));
  } else if (info[0].IsString()) {
    std::string strInput = info[0].As<Napi::String>().Utf8Value();
    buf = (const char *)strInput.c_str();
    len = (size_t)strInput.length();
    return Napi::Number::New(env, table_calculate(crc, buf, len));
  } else {
    throw Napi::TypeError::New(env, "input is not a string/buffer!");
  }
}

/**
 * Initialize the module
 */
Napi::Object init(Napi::Env env, Napi::Object exports) {
  // initialize the required pieces
  table_init();

  // export the binding interface
  exports["hardware_support"] = Napi::Boolean::New(env, sse42_is_available());
  exports["sse42_crc"] = Napi::Function::New(env, sse42_crc);
  exports["table_crc"] = Napi::Function::New(env, table_crc);

  return exports;
};

NODE_API_MODULE(crc32c, init);
