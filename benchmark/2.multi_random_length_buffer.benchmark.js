/**
 * @file Benchmarks using multiple random-length buffers
 *
 * @author Anand Suresh <anand.suresh@gmail.com>
 * @copyright Copyright (C) 2007-2015 Voxer LLC. All rights reserved.
 * @license MIT
 */

var Sse4Crc32 = require('../sse4_crc32');
var jsCrc32 = require('crc32');
var crc, startTime, endTime, i;



/**
 * Maximum length of a test buffer
 * @type {Number}
 */
var MAX_STRING_LENGTH = 4096;


/**
 * A list of valid characters used to compose the test buffers
 * @type {String}
 */
var VALID_CHARS = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()+_=-[]{}\\|;\':",./<>?`~ ';


/**
 * The number of times to invoke the CRC function for the benchmarks
 * @type {Number}
 */
var INVOCATIONS = 100000;



// Generate the test buffers
var testBuffers = [];
var avgBufferLength = 0;

console.log(INVOCATIONS + ' calls to calculate CRC on random-length buffers upto ' + MAX_STRING_LENGTH + ' bytes long...');
for (i = 0; i < INVOCATIONS; i++) {
    var len = Math.random() * MAX_STRING_LENGTH;
    var str = '';

    for (var j = 0; j < len; j++) {
        str += VALID_CHARS.charAt(Math.random() * VALID_CHARS.length);
    }
    testBuffers.push(new Buffer(str));
    avgBufferLength += len;
}
avgBufferLength /= INVOCATIONS;
console.log('\tAvg. buffer length: %d bytes', parseInt(avgBufferLength));



// Hardware-based CRC-32C
startTime = process.hrtime();
testBuffers.forEach(function(str) {
    crc = Sse4Crc32.calculateOnHardware(str);
});
endTime = process.hrtime(startTime);
console.log('\tHardware-based CRC-32C: %d ms', (endTime[0] * 1e9 + endTime[1]) / (1000 * 1000));


// Software-based CRC-32C
startTime = process.hrtime();
testBuffers.forEach(function(str) {
    crc = Sse4Crc32.calculateInSoftware(str);
});
endTime = process.hrtime(startTime);
console.log('\tSoftware-based CRC-32C: %d ms', (endTime[0] * 1e9 + endTime[1]) / (1000 * 1000));


// Pure JS CRC-32C (table-based)
startTime = process.hrtime();
testBuffers.forEach(function(str) {
    crc = jsCrc32(str, false);
});
endTime = process.hrtime(startTime);
console.log('\tPure JS CRC-32C (table-based): %d ms', (endTime[0] * 1e9 + endTime[1]) / (1000 * 1000));


// Pure JS CRC-32C (direct)
startTime = process.hrtime();
testBuffers.forEach(function(str) {
    crc = jsCrc32(str, true);
});
endTime = process.hrtime(startTime);
console.log('\tPure JS CRC-32C (direct): %d ms', (endTime[0] * 1e9 + endTime[1]) / (1000 * 1000));
