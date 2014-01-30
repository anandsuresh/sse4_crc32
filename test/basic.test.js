var SSE4CRC32 = require("../sse4_crc32"),
    tap = require("tap"),
    test = tap.test;


var TEST_CASES = [
    {
        input : "SSE4-CRC32: A hardware accelerated CRC32 implementation for node.js",
        output: 3039989317
    },
    {
        input : new Buffer("SSE4-CRC32: A hardware accelerated CRC32 implementation for node.js"),
        output: 3039989317
    },
    {
        input : [ "SSE4-CRC32: ", "A hardware accelerated CRC32 implementation ", "for node.js" ],
        output: 3039989317
    }
];


/**
 * Runs basic tests
 */
test("Basic Tests", function (t) {
    TEST_CASES.forEach(function (test_case) {
        if (Array.isArray(test_case.input)) {
            var test_crc = new SSE4CRC32.CRC32();
            test_case.input.forEach(function (str) {
                test_crc.update(str);
            });
            t.equals(test_crc.crc(), test_case.output, "CRC32 for [ " + test_case.input + " ] should be " + test_case.output);
        } else {
            t.equals(SSE4CRC32.calculate(test_case.input), test_case.output, "CRC32 for " + test_case.input + " should be " + test_case.output);
        }
    });
    t.end();
});
