{
    "includes": [ "common.gypi" ],
    "targets":  [
        {
            "target_name":    "sse4_crc32",
            "sources":        [
                "src/sse4_crc32.cpp",
                "src/crc32c.c"
            ],
            "include_dirs" : [
                "<!(node -e \"require('nan')\")"
            ],
            "xcode_settings": {
                "GCC_ENABLE_SSE42_EXTENSIONS": "YES"
            },
            "cflags":         [ "-msse4.2" ]
        }
    ]
}
