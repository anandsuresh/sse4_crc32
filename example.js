var myaddon = require("./build/Release/sse4_crc32.node"),
    str1 = "Anand Suresh",
    str2 = "Voxer Inc. rocks!!";

console.log("generateCRC32(\"" + str1 + "\") = " + myaddon.generate(str1));
console.log("generateCRC32(\"" + str2 + "\") = " + myaddon.generate(str2));
