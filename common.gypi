{
    "target_defaults": {
        "default_configuration": "Release",
        "configurations":        {
            "Debug":   {
                "defines":  [ "DEBUG" ],
                "defines!": [ "NDEBUG" ]
            },
            "Release": {
                "defines": [ "NDEBUG" ]
            }
        }
    }
}
