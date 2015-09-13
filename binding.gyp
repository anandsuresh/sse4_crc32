{
    "includes": [ "common.gypi" ],
    "targets":  [
        {
            "target_name":    "crc32c_sse42",
            "type":           "static_library",
            "sources":        [
                "src/crc32c_sse42.cpp"
            ],
            "xcode_settings": {
                "GCC_ENABLE_SSE42_EXTENSIONS": "YES"
            },
            "cflags":         [ "-msse4.2" ]
        },
        {
            "target_name":    "sse4_crc32",
            "sources":        [
                "src/crc32c.cpp"
            ],
            "include_dirs":   [
                "<!(node -e \"require('nan')\")"
            ],
            "dependencies":   [
                "crc32c_sse42"
            ]
        }
    ]
}
