var myaddon = require("./build/Release/sse4_crc32.node"),
    str1 = "Anand Suresh",
    str2 = "Voxer Inc. rocks!";

console.log("fastCRC32(\"" + str1 + "\") = " + myaddon.generateFast(str1));
console.log("fastCRC32(\"" + str2 + "\") = " + myaddon.generateFast(str2));
