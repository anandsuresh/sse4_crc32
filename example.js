var myaddon = require("./build/Release/sse4_crc32.node"),
    str1 = "myname",
    str2 = "myname12";

console.log("slowCRC32(\"" + str1 + "\") = " + myaddon.generateSlow(str1));
console.log("fastCRC32(\"" + str1 + "\") = " + myaddon.generateFast(str1));

console.log("slowCRC32(\"" + str2 + "\") = " + myaddon.generateSlow(str2));
console.log("fastCRC32(\"" + str2 + "\") = " + myaddon.generateFast(str2));
