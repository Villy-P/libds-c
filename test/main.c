#define DS_C_IMPLEMENTATION
#include "libds-c.h"

#include <assert.h>
#include <stdio.h>

static const int TEST_ARRAY_INITIAL_CAPACITY = 10;
static const int TEST_ARRAY_RESIZE_CAPACITY = 20;
static const int TEST_ARRAY_PUSH_COUNT = 15;
static const int TEST_ARRAY_INSERT_VALUE = 99;

static void test_array() {
    // Array Creation
    ds_array* arr = ds_array_create(TEST_ARRAY_INITIAL_CAPACITY);
    assert(arr != NULL);
    assert(arr->capacity == TEST_ARRAY_INITIAL_CAPACITY);
    assert(arr->length == 0);

    // Resize
    assert(ds_array_resize(arr, TEST_ARRAY_RESIZE_CAPACITY) == true);
    assert(arr->capacity == TEST_ARRAY_RESIZE_CAPACITY);

    // Push elements
    int test_values[TEST_ARRAY_PUSH_COUNT];
    for (int i = 0; i < TEST_ARRAY_PUSH_COUNT; ++i) {
        test_values[i] = i;
        assert(ds_array_push(arr, &test_values[i]) == true);
        assert(arr->length == (size_t)(i + 1));
    }

    // Get elements
    for (int i = 0; i < TEST_ARRAY_PUSH_COUNT; ++i) {
        int* retrieved = DSM_ARRAY_GET(arr, i, int*);
        assert(retrieved != NULL);
        assert(*retrieved == i);
    }

    // Insert element
    int insert_value = TEST_ARRAY_INSERT_VALUE;
    assert(ds_array_insert(arr, 5, &insert_value) == true);
    int* retrieved_insert = DSM_ARRAY_GET(arr, 5, int*);
    assert(retrieved_insert != NULL);
    assert(*retrieved_insert == TEST_ARRAY_INSERT_VALUE);

    // Remove element
    assert(ds_array_remove(arr, 5) != NULL);
    int* retrieved_after_remove = DSM_ARRAY_GET(arr, 5, int*);
    assert(retrieved_after_remove != NULL);
    assert(*retrieved_after_remove == 5);

    // Pop element
    int* popped_value = ds_array_pop(arr);
    assert(popped_value != NULL);
    assert(*popped_value == 14);

    // Clear array
    ds_array_clear(arr);
    assert(arr->length == 0);

    // Clean up
    ds_array_destroy(arr);
    printf("test_array passed\n");
}

int main() {
    test_array();
    printf("All tests passed\n");
    return 0;
}