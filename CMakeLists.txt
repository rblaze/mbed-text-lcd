add_library(mbed-text-lcd INTERFACE)

target_include_directories(mbed-text-lcd
    INTERFACE
        .
)

target_sources(mbed-text-lcd
    INTERFACE
        ST7036i.cpp
)
