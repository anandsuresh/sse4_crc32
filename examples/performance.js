var crc32 = require("../sse4_crc32"),
    js_crc32 = require("crc32"),
    start_time, crc;


var TEST_STRING = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Morbi mollis cursus metus vel tristique. Proin congue massa massa, a malesuada dolor ullamcorper a. Nulla eget leo vel orci venenatis placerat. Donec semper condimentum justo, vel sollicitudin dolor consequat id. Nunc sed aliquet felis, eget congue nisi. Mauris eu justo suscipit, elementum turpis ut, molestie tellus. Mauris ornare rutrum fringilla. Nulla dignissim luctus pretium. Nullam nec eros hendrerit sapien pellentesque sollicitudin. Integer eget ligula dui. Mauris nec cursus nibh. Nunc interdum elementum leo, eu sagittis eros sodales nec. Duis dictum nulla sed tincidunt malesuada. Quisque in vulputate sapien. Sed sit amet tellus a est porta rhoncus sed eu metus. Mauris non pulvinar nisl, volutpat luctus enim. Suspendisse est nisi, sagittis at risus quis, ultricies rhoncus sem. Donec ullamcorper purus eget sapien facilisis, eu eleifend felis viverra. Suspendisse elit neque, semper aliquet neque sed, egestas tempus leo. Duis condimentum turpis duis.",
    REPEATS = 100000;

/**
 * Runs a performance test
 */
console.log("String length: " + TEST_STRING.length);

// Test using SSE CRC32
start_time = Date.now();
for (var i = 0; i < REPEATS; i++) {
    crc = crc32.calculateCRC32(TEST_STRING);
}
console.log("Runtime for " + REPEATS + " invocations of SSE4.2 based CRC32: " + (Date.now() - start_time) + "ms. CRC: " + crc);

// Test using pure JS CRC32
start_time = Date.now();
for (var i = 0; i < REPEATS; i++) {
    crc = js_crc32(TEST_STRING);
}
console.log("Runtime for " + REPEATS + " invocations of pure JS based CRC32: " + (Date.now() - start_time) + "ms. CRC: " + crc);
