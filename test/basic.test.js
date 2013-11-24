var crc32 = require("../sse4_crc32"),
    tap = require("tap"),
    test = tap.test;


var TEST_CASES = [
    {
        input : "Anand Suresh",
        output: 3383959586
    },
    {
        input : "Voxer Inc. rocks!!",
        output: 1843917851
    },
    {
        input : new Buffer("Anand Suresh"),
        output: 3383959586
    },
    {
        input : new Buffer("Voxer Inc. rocks!!"),
        output: 1843917851
    }
];


/**
 * Runs basic tests
 */
test("Basic Tests", function (t) {
    TEST_CASES.forEach(function (test_case) {
        t.equals(crc32.calculateCRC32(test_case.input), test_case.output, "CRC32 for " + test_case.input + " should be " + test_case.output);
    });
    t.end();
});
