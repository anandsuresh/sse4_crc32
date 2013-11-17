var crc32 = require("../sse4_crc32"),
    tap = require("tap"),
    test = tap.test;


var
TEST_CASES = {
    "Anand Suresh":       3383959586,
    "Voxer Inc. rocks!!": 1843917851
};

/**
 * Runs basic tests
 */
test("Basic Tests", function (t) {
    Object.keys(TEST_CASES).forEach(function (str) {
        t.equals(crc32.calculateCRC32(str), TEST_CASES[str], "CRC32 for " + str + " should be " + TEST_CASES[str]);
    });
    t.end();
});
