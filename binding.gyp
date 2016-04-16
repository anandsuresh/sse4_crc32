{
    "includes": [ "./common.gypi" ],
    "conditions":[
        ['target_arch in "ia32 x32 x64 x86 x86_64"', {
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
                }
            ]
        }]
    ],
    "targets":  [
        {
            "target_name":    "sse4_crc32",
            "sources":        [
                "src/crc32c.cpp"
            ],
            "include_dirs":   [
                "<!(node -e \"require('nan')\")"
            ],
            "conditions":[
                ['target_arch in "ia32 x32 x64 x86 x86_64"', {
                    "dependencies":   [
                        "crc32c_sse42"
                    ]
                }]
            ]
        }
    ]
}
