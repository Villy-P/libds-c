#define LIBDS_C_IMPLEMENTATION
#include "libds-c.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static const int TEST_ARRAY_INITIAL_CAPACITY = 10;

static void test_array_creation() {
    ds_array* arr = ds_array_create(TEST_ARRAY_INITIAL_CAPACITY);
    assert(arr != NULL);
    assert(arr->capacity == TEST_ARRAY_INITIAL_CAPACITY);
    assert(arr->length == 0);
    
    // Clean up
    free(arr->data);
    free(arr);
    printf("test_array_creation passed\n");
}

int main() {
    test_array_creation();
    printf("All tests passed\n");
    return 0;
}