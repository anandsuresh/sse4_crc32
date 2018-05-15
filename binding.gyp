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
            'cflags!' : ['-fno-exceptions'],
            'cflags_cc!' : ['-fno-exceptions'],
            "include_dirs":   [
                "<!@(node -p \"require('node-addon-api').include\")"
            ],
            'dependencies' : ["<!(node -p \"require('node-addon-api').gyp\")"],
            "conditions":[
                ['target_arch in "ia32 x32 x64 x86 x86_64"', {
                    "dependencies":   [
                        "crc32c_sse42"
                    ]
                }],
                [ 'OS=="win"', {
                    "msvs_settings" : {
                        "VCCLCompilerTool" : {
                            "ExceptionHandling" : 1
                        }
                    }
                }],
                [ 'OS=="mac"', {
                    "xcode_settings": {
                        "CLANG_CXX_LIBRARY" : "libc++",
                        'GCC_ENABLE_CPP_EXCEPTIONS' : 'YES',
                        'MACOSX_DEPLOYMENT_TARGET' : '10.7'
                    }
                }]
            ]
        }
    ]
}
