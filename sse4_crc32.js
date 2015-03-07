/**
 * @file Provides hardware-based CRC-32C calculation functionality, with software fallback
 *
 * @author Anand Suresh <anand.suresh@gmail.com>
 * @copyright Copyright (C) 2007-2015 Voxer LLC. All rights reserved.
 * @license MIT
 */
var Sse4Crc32 = require("bindings")("sse4_crc32");



/**
 * Calculates CRC in software mode
 *
 * @param {String|Buffer} input The input string/buffer for which the CRC is to be calculated
 * @param {Number} [initialCrc=0] An optional initial CRC
 * @returns {Number}
 */
function swCrc32c(input, initialCrc) {
    return Sse4Crc32.swCrc(input, initialCrc || 0);
}


/**
 * Calculates CRC in hardware mode
 *
 * @param {String|Buffer} input The input string/buffer for which the CRC is to be calculated
 * @param {Number} [initialCrc=0] An optional initial CRC
 * @returns {Number}
 */
function hwCrc32c(input, initialCrc) {
    if (!Sse4Crc32.isHardwareCrcSupported) throw new Error('Hardware CRC-32C not supported!');

    return Sse4Crc32.hwCrc(input, initialCrc || 0);
}


/**
 * Used to calculate 32-bit CRC for single instances of strings and/or buffers
 *
 * This function automatically determines the best mode of operation (h/w vs. s/w implementation).
 *
 * @param {String|Buffer} input The input string for which the CRC is to be calculated
 * @param {Number} [initialCrc=0] An optional initial CRC
 * @returns {Number}
 */
function calculate(input, initialCrc) {
    var crcFunction = Sse4Crc32.isHardwareCrcSupported ? Sse4Crc32.hwCrc : Sse4Crc32.swCrc;

    return crcFunction(input, initialCrc || 0);
}



/**
 * Defines a progressive CRC-32C calculator
 *
 * @param {String|Buffer} input The input string for which the CRC is to be calculated
 * @param {Number} [initialCrc=0] An optional initial CRC
 * @constructor
 */
function Crc32C(input, initialCrc) {
    this.crc32c = initialCrc || 0;

    if (input) this.update(input);
}


/**
 * Progressively calculates the 32-bit CRC
 *
 * @param input Additional input to calculate the CRC for
 * @returns {Crc32C}
 */
Crc32C.prototype.update = function(input) {
    this.crc32c = calculate(input, this.crc32c);
    return this;
};


/**
 * Returns the 32-bit CRC
 *
 * @returns {Number}
 */
Crc32C.prototype.crc = function() {
    return this.crc32c;
};


/**
 * Export the required functions/classes
 * @type {Object}
 */
module.exports = {
    isHardwareCrcSupported: Sse4Crc32.isHardwareCrcSupported,
    calculateInSoftware   : swCrc32c,
    calculateOnHardware   : hwCrc32c,

    CRC32    : Crc32C,
    calculate: calculate
};
