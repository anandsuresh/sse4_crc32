/**
 * @file Example usage of the SSE4CRC32 library
 *
 * @author Anand Suresh <anand.suresh@gmail.com>
 * @copyright Copyright (C) 2007-2015 Voxer LLC. All rights reserved.
 * @license MIT
 */

var Sse4Crc32 = require('../sse4_crc32');


// Usage for calculating CRC32 for strings
var str = 'SSE4-CRC32: A hardware accelerated CRC32 implementation for node.js';
console.log('crc("' + str + '") = ' + Sse4Crc32.calculate(str));


// Usage for calculating CRC32 for buffers
var buf = new Buffer('SSE4-CRC32: A hardware accelerated CRC32 implementation for node.js');
console.log('crc("' + buf + '") = ' + Sse4Crc32.calculate(buf));


// Usage for calculating progressive CRC32
var crc32 = new Sse4Crc32.CRC32(),
    array = [ 'SSE4-CRC32: ', 'A hardware accelerated CRC32 implementation ', 'for node.js' ];
array.forEach(function (str) {
    crc32.update(str);
});
console.log('crc([' + array + ']) = ' + crc32.crc());
