var SSE4CRC32 = require("../sse4_crc32"),
    js_crc32 = require("crc32"),
    crc, start_time, end_time;


var MAX_STRING_LENGTH = 4096,
    VALID_CHARS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()+_=-[]{}\\|;':\",./<>?`~ ",
    INVOCATIONS = 100000,
    TEST_BUFFERS = [],
    avg_len = 0;


// Generate random strings
console.log(INVOCATIONS + " calls to calculate CRC on random length buffers upto " + MAX_STRING_LENGTH + " bytes long...");
for (var i = 0; i < INVOCATIONS; i++) {
    var len = Math.random() * MAX_STRING_LENGTH,
        str = "";

    for (var j = 0; j < len; j++) {
        str += VALID_CHARS.charAt(Math.random() * VALID_CHARS.length);
    }
    TEST_BUFFERS.push(new Buffer(str));
    avg_len += len;
}
avg_len /= INVOCATIONS;
console.log("\tAvg. buffer length: " + parseInt(avg_len) + " bytes");


// Test using SSE CRC32
start_time = Date.now();
TEST_BUFFERS.forEach(function (str) {
    crc = SSE4CRC32.calculate(str);
});
end_time = Date.now();
console.log("\tSSE4.2 based CRC32: " + (end_time - start_time) + "ms.");


// Test using pure JS CRC32 (table-based)
start_time = Date.now();
TEST_BUFFERS.forEach(function (str) {
    crc = js_crc32(str, false);
});
end_time = Date.now();
console.log("\tPure JS based CRC32 (table-based): " + (end_time - start_time) + "ms.");


// Test using pure JS CRC32 (direct)
start_time = Date.now();
TEST_BUFFERS.forEach(function (str) {
    crc = js_crc32(str, true);
});
end_time = Date.now();
console.log("\tPure JS based CRC32 (direct): " + (end_time - start_time) + "ms.\n");
