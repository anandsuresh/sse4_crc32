var SSE4CRC32 = require("../sse4_crc32"),
    js_crc32 = require("crc32"),
    crc, start_time, end_time;


/**
 * Runs a performance test on a 1kb cached string
 */
(function cached_string_test() {
    var TEST_STRING = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Morbi mollis cursus metus vel tristique. Proin congue massa massa, a malesuada dolor ullamcorper a. Nulla eget leo vel orci venenatis placerat. Donec semper condimentum justo, vel sollicitudin dolor consequat id. Nunc sed aliquet felis, eget congue nisi. Mauris eu justo suscipit, elementum turpis ut, molestie tellus. Mauris ornare rutrum fringilla. Nulla dignissim luctus pretium. Nullam nec eros hendrerit sapien pellentesque sollicitudin. Integer eget ligula dui. Mauris nec cursus nibh. Nunc interdum elementum leo, eu sagittis eros sodales nec. Duis dictum nulla sed tincidunt malesuada. Quisque in vulputate sapien. Sed sit amet tellus a est porta rhoncus sed eu metus. Mauris non pulvinar nisl, volutpat luctus enim. Suspendisse est nisi, sagittis at risus quis, ultricies rhoncus sem. Donec ullamcorper purus eget sapien facilisis, eu eleifend felis viverra. Suspendisse elit neque, semper aliquet neque sed, egestas tempus leo. Duis condimentum turpis duis.",
        INVOCATIONS = 10000;

    console.log(INVOCATIONS + " invocations of CRC32 on a " + TEST_STRING.length + " byte string...");

    // Test using SSE CRC32
    start_time = Date.now();
    for (var i = 0; i < INVOCATIONS; i++) {
        crc = SSE4CRC32.calculate(TEST_STRING);
    }
    end_time = Date.now();
    console.log("\tSSE4.2 based CRC32: " + (end_time - start_time) + "ms.");

    // Test using pure JS CRC32
    start_time = Date.now();
    for (var i = 0; i < INVOCATIONS; i++) {
        crc = js_crc32(TEST_STRING);
    }
    end_time = Date.now();
    console.log("\tPure JS based CRC32: " + (end_time - start_time) + "ms.\n");
})();


(function random_string_test() {
    var MAX_STRING_LENGTH = 4096,
        VALID_CHARS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()+_=-[]{}\\|;':\",./<>?`~ ",
        INVOCATIONS = 10000,
        RANDOM_STRINGS = [],
        avg_len = 0;

    // Generate random strings
    console.log(INVOCATIONS + " invocations of CRC32 on random length strings upto " + MAX_STRING_LENGTH + " bytes long...");
    for (var i = 0; i < INVOCATIONS; i++) {
        var len = Math.random() * MAX_STRING_LENGTH,
            str = "";

        for (var j = 0; j < len; j++) {
            str += VALID_CHARS.charAt(Math.random() * VALID_CHARS.length);
        }
        RANDOM_STRINGS.push(str);
        avg_len += len;
    }
    avg_len /= INVOCATIONS;
    console.log("\tAvg. string length: " + parseInt(avg_len) + " bytes");

    // Test using SSE CRC32
    start_time = Date.now();
    RANDOM_STRINGS.forEach(function (str) {
        crc = SSE4CRC32.calculate(str);
    });
    end_time = Date.now();
    console.log("\tSSE4.2 based CRC32: " + (end_time - start_time) + "ms.");

    // Test using pure JS CRC32
    start_time = Date.now();
    RANDOM_STRINGS.forEach(function (str) {
        crc = js_crc32(str);
    });
    end_time = Date.now();
    console.log("\tPure JS based CRC32: " + (end_time - start_time) + "ms.\n");
})();