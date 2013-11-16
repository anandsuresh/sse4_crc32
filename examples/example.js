/**
 * @file example.js
 * @brief Example code using the CRC32 functions
 * 
 * @author Anand Suresh <anandsuresh@gmail.com>
 */
var myaddon = require("../sse4_crc32"), str1 = "Anand Suresh", str2 = "Voxer Inc. rocks!!";

console.log("calculateCRC32(\"" + str1 + "\") = " + myaddon.calculateCRC32(str1));
console.log("calculateCRC32(\"" + str2 + "\") = " + myaddon.calculateCRC32(str2));
