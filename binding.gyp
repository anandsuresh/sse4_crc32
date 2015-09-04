{
    "includes": [ "common.gypi" ],
    "targets":  [
        {
            "target_name":    "sse4_crc32_hw",
            "type":           "static_library",
            "sources":        [
                "src/sse4_crc32.cpp"
            ],
            "xcode_settings": {
                "GCC_ENABLE_SSE42_EXTENSIONS": "YES"
            },
            "cflags":         [ "-msse4.2" ]
        },
        {
            "target_name":    "sse4_crc32",
            "sources":        [
                "src/crc32.cpp"
            ],
            "include_dirs":   [
                "<!(node -e \"require('nan')\")"
            ],
            "dependencies":   [
                "sse4_crc32_hw"
            ]
        }
    ]
}
