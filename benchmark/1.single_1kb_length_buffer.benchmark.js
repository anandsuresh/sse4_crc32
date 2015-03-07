/**
 * @file Benchmarks using a single 1kB buffer
 *
 * @author Anand Suresh <anand.suresh@gmail.com>
 * @copyright Copyright (C) 2007-2015 Voxer LLC. All rights reserved.
 * @license MIT
 */

var Sse4Crc32 = require('../sse4_crc32');
var jsCrc32 = require('crc32');
var crc, startTime, endTime, i;



/**
 * The input buffer for the CRC-benchmarks
 * @type {Buffer}
 */
var TEST_BUFFER = new Buffer('Lorem ipsum dolor sit amet, consectetur adipiscing elit. Morbi mollis cursus metus vel tristique. Proin congue massa massa, a malesuada dolor ullamcorper a. Nulla eget leo vel orci venenatis placerat. Donec semper condimentum justo, vel sollicitudin dolor consequat id. Nunc sed aliquet felis, eget congue nisi. Mauris eu justo suscipit, elementum turpis ut, molestie tellus. Mauris ornare rutrum fringilla. Nulla dignissim luctus pretium. Nullam nec eros hendrerit sapien pellentesque sollicitudin. Integer eget ligula dui. Mauris nec cursus nibh. Nunc interdum elementum leo, eu sagittis eros sodales nec. Duis dictum nulla sed tincidunt malesuada. Quisque in vulputate sapien. Sed sit amet tellus a est porta rhoncus sed eu metus. Mauris non pulvinar nisl, volutpat luctus enim. Suspendisse est nisi, sagittis at risus quis, ultricies rhoncus sem. Donec ullamcorper purus eget sapien facilisis, eu eleifend felis viverra. Suspendisse elit neque, semper aliquet neque sed, egestas tempus leo. Duis condimentum turpis duis.');


/**
 * The number of times to invoke the CRC function for the benchmarks
 * @type {Number}
 */
var INVOCATIONS = 100000;



console.log(INVOCATIONS + ' calls to calculate CRC on a ' + TEST_BUFFER.length + ' byte buffer...');



// Hardware-based CRC-32C
startTime = process.hrtime();
for (i = 0; i < INVOCATIONS; i++) {
    crc = Sse4Crc32.calculateOnHardware(TEST_BUFFER);
}
endTime = process.hrtime(startTime);
console.log('\tHardware-based CRC-32C: %d ms', (endTime[0] * 1e9 + endTime[1]) / (1000 * 1000));


// Software-based CRC-32C
startTime = process.hrtime();
for (i = 0; i < INVOCATIONS; i++) {
    crc = Sse4Crc32.calculateInSoftware(TEST_BUFFER);
}
endTime = process.hrtime(startTime);
console.log('\tSoftware-based CRC-32C: %d ms', (endTime[0] * 1e9 + endTime[1]) / (1000 * 1000));


// Pure JS CRC-32C (table-based)
startTime = process.hrtime();
for (i = 0; i < INVOCATIONS; i++) {
    crc = jsCrc32(TEST_BUFFER, false);
}
endTime = process.hrtime(startTime);
console.log('\tPure JS CRC-32C (table-based): %d ms', (endTime[0] * 1e9 + endTime[1]) / (1000 * 1000));


// Pure JS CRC-32C (direct)
startTime = process.hrtime();
for (i = 0; i < INVOCATIONS; i++) {
    crc = jsCrc32(TEST_BUFFER, true);
}
endTime = process.hrtime(startTime);
console.log('\tPure JS CRC-32C (direct): %d ms', (endTime[0] * 1e9 + endTime[1]) / (1000 * 1000));
