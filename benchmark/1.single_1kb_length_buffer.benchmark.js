var SSE4CRC32 = require("../sse4_crc32"),
    js_crc32 = require("crc32"),
    crc, start_time, end_time;


var TEST_BUFFER = new Buffer("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Morbi mollis cursus metus vel tristique. Proin congue massa massa, a malesuada dolor ullamcorper a. Nulla eget leo vel orci venenatis placerat. Donec semper condimentum justo, vel sollicitudin dolor consequat id. Nunc sed aliquet felis, eget congue nisi. Mauris eu justo suscipit, elementum turpis ut, molestie tellus. Mauris ornare rutrum fringilla. Nulla dignissim luctus pretium. Nullam nec eros hendrerit sapien pellentesque sollicitudin. Integer eget ligula dui. Mauris nec cursus nibh. Nunc interdum elementum leo, eu sagittis eros sodales nec. Duis dictum nulla sed tincidunt malesuada. Quisque in vulputate sapien. Sed sit amet tellus a est porta rhoncus sed eu metus. Mauris non pulvinar nisl, volutpat luctus enim. Suspendisse est nisi, sagittis at risus quis, ultricies rhoncus sem. Donec ullamcorper purus eget sapien facilisis, eu eleifend felis viverra. Suspendisse elit neque, semper aliquet neque sed, egestas tempus leo. Duis condimentum turpis duis."),
    INVOCATIONS = 100000;

console.log(INVOCATIONS + " calls to calculate CRC on a " + TEST_BUFFER.length + " byte buffer...");


// Test using SSE CRC32
start_time = Date.now();
for (var i = 0; i < INVOCATIONS; i++) {
    crc = SSE4CRC32.calculate(TEST_BUFFER);
}
end_time = Date.now();
console.log("\tSSE4.2 based CRC32: " + (end_time - start_time) + "ms.");


// Test using pure JS CRC32 (table-based)
start_time = Date.now();
for (var i = 0; i < INVOCATIONS; i++) {
    crc = js_crc32(TEST_BUFFER, false);
}
end_time = Date.now();
console.log("\tPure JS based CRC32 (table-based): " + (end_time - start_time) + "ms.");


// Test using pure JS CRC32 (direct)
start_time = Date.now();
for (var i = 0; i < INVOCATIONS; i++) {
    crc = js_crc32(TEST_BUFFER, true);
}
end_time = Date.now();
console.log("\tPure JS based CRC32 (direct): " + (end_time - start_time) + "ms.\n");
