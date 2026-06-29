#define LIBDS_C_IMPLEMENTATION
#include "libds-c.h"

#include <assert.h>
#include <stdio.h>

static const int TEST_ARRAY_INITIAL_CAPACITY = 10;

static void test_array() {
    // Array Creation
    ds_array* arr = ds_array_create(TEST_ARRAY_INITIAL_CAPACITY);
    assert(arr != NULL);
    assert(arr->capacity == TEST_ARRAY_INITIAL_CAPACITY);
    assert(arr->length == 0);

    // Resize
    assert(ds_array_resize(arr, 20) == true);
    assert(arr->capacity == 20);
    
    // Clean up
    ds_array_destroy(arr);
    printf("test_array passed\n");
}

int main() {
    test_array();
    printf("All tests passed\n");
    return 0;
}