add_library(common_flags INTERFACE)

target_compile_options(common_flags INTERFACE
    -Wall 
    -Wextra 
    -g 
    -O0
    $<$<PLATFORM_ID:Windows>:-gcodeview>
    $<$<CONFIG:Debug>:-fdiagnostics-color=always>
)

target_link_options(common_flags INTERFACE
    -g
    $<$<PLATFORM_ID:Windows>:-Wl,--export-all-symbols>
    $<$<PLATFORM_ID:Windows>:-Wl,--enable-auto-image-base>
)