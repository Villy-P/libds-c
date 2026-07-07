add_library(libds-c INTERFACE)
add_library(libds-c::libds-c ALIAS libds-c)
target_include_directories(libds-c INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)