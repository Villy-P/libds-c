# libds-c

`libds-c` is a header-only library for C projects that implements a wide variety of data structures alongside strings.

## Roadmap

| Data Structure | Status      |
| -------------- | ----------- |
| Dynamic Array  | Finished    |
| Stack          | Finished    |
| String         | In Progress |
| Hashmap        | Not started |
| Hashset        | Not started |

Other data structures will be added in the future as well.

## Installing libds-c

### Via manual installation

Copy the `include` directory into your project, then you can use all functions as normal.

### Via CMake

Add this snippet to your `CMakeLists.txt`:

``` cmake
include(FetchContent)

FetchContent_Declare(libds-c
    GIT_REPOSITORY https://github.com/Villy-P/libds-c.git
    GIT_TAG        main # consider pinning to a release tag or commit SHA for reproducible builds
)
FetchContent_MakeAvailable(libds-c)

target_link_libraries(your_target PRIVATE libds-c::libds-c)
```

Replace `your_target` with the name of your project. All `libds-c` headers will be available to it.

## Using libds-c

By default, libds-c only includes function declarations. 
To include the function implementations, define `DS_C_IMPLEMENTATION` in exactly **one** C file:

``` c
#define DS_C_IMPLEMENTATION
#include <libds-c.h>
```

Additionally, if you would like to have stack functions, define the `DS_DEFINE_STACK_IMPLEMENTATION` macro.

Alternatively, you can add these with `CMake` using `add_definitions`:

``` cmake
add_definitions(-DDS_DEFINE_STACK_IMPLEMENTATION -DDS_C_IMPLEMENTATION)
```

## Type safe data structures

By default, `libds-c` data structures are type-anonymous, so any type can be inserted into any data structure.

To add type-safety to data structures, use the `DS_DEFINE_` macro in a header file:

``` c
DS_DEFINE_ARRAY(int, ds_int_array, NULL, NULL)

// Usage
ds_int_array* arr = ds_int_array_create(10);
ds_int_array_push(arr, 2); // Works
ds_int_array_push(arr, 'c'); // Compiler error: incorrect type
```

## Error Handling

By default, `libds-c` will return error codes as found in `DS_STATUS` if a runtime error occurs. By defining the `DS_THROWS_ERROR` macro, all critical errors will instead stop the program.

## License

`libds-c` is licensed under the [MIT License](./LICENSE).
