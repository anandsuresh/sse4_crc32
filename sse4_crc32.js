var sse4_crc32 = require("bindings")("sse4_crc32");


/**
 * Defines a progressive 32-bit CRC calculator
 *
 * @param input The input string for which the CRC is to be calculated
 * @param initial_crc The initial CRC passed in [optional]
 *
 * @constructor
 */
function CRC32(input, initial_crc) {
    this.crc32 = initial_crc || 0;

    if (input) {
        this.update(input);
    }
}

/**
 * Progressively calculates the 32-bit CRC
 *
 * @param input Additional input to calculate the CRC for
 */
CRC32.prototype.update = function (input) {
    this.crc32 = sse4_crc32.calculate(input, this.crc32);
    return this.crc32;
};

/**
 * Returns the 32-bit CRC
 *
 * @returns {Integer}
 */
CRC32.prototype.crc = function () {
    return this.crc32;
};


/**
 * Used to calculate 32-bit CRC for single instances of strings and/or buffers
 *
 * @param input The input string for which the CRC is to be calculated
 * @param initial_crc The initial CRC passed in [optional]
 *
 * @returns {Integer}
 */
function calculate(input, initial_crc) {
    return sse4_crc32.calculate(input, initial_crc || 0);
}


module.exports = {
    CRC32    : CRC32,
    calculate: calculate
};
