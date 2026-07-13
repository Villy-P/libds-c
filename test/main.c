#define DS_C_IMPLEMENTATION
// #define DS_THROWS_ERROR
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "ds_array.h"
#include "ds_common.h"

static const int TEST_ARRAY_INITIAL_CAPACITY = 10;
static const int TEST_ARRAY_RESIZE_CAPACITY = 20;
static const int TEST_ARRAY_PUSH_COUNT = 15;
static const int TEST_ARRAY_INSERT_VALUE = 99;
static const int TEST_ARRAY_INSERT_INDEX = 5;

DS_DEFINE_ARRAY(int, ds_int_array, NULL, NULL);

static void test_array_basic() {
    ds_int_array* arr = ds_int_array_create(TEST_ARRAY_INITIAL_CAPACITY);
    assert(arr != NULL);
    assert(arr->capacity == (size_t)TEST_ARRAY_INITIAL_CAPACITY);
    assert(arr->length == 0);

    assert(ds_int_array_resize(arr, TEST_ARRAY_RESIZE_CAPACITY) ==
           DS_STATUS_OK);
    assert(arr->capacity == (size_t)TEST_ARRAY_RESIZE_CAPACITY);

    int test_values[TEST_ARRAY_PUSH_COUNT];
    for (int i = 0; i < TEST_ARRAY_PUSH_COUNT; ++i) {
        test_values[i] = i;
        assert(ds_int_array_push(arr, test_values[i]) == DS_STATUS_OK);
        assert(arr->length == (size_t)(i + 1));
    }

    for (int i = 0; i < TEST_ARRAY_PUSH_COUNT; ++i) {
        int retrieved = ds_int_array_get(arr, i);
        assert(retrieved == i);
    }

    int insert_value = TEST_ARRAY_INSERT_VALUE;
    assert(ds_int_array_insert(arr, TEST_ARRAY_INSERT_INDEX, insert_value) ==
           DS_STATUS_OK);
    int retrieved_insert = ds_int_array_get(arr, TEST_ARRAY_INSERT_INDEX);
    assert(retrieved_insert == TEST_ARRAY_INSERT_VALUE);

    assert(ds_int_array_remove(arr, TEST_ARRAY_INSERT_INDEX, NULL) ==
           DS_STATUS_OK);

    ds_int_array_clear(arr);
    assert(arr->length == 0);

    ds_int_array_destroy(arr);
    printf("test_array_basic passed\n");
}

static void test_array_growth_from_small_capacity() {
    ds_int_array* arr = ds_int_array_create(1);
    int var1 = 1;
    int var2 = 2;
    int var3 = 3;
    assert(ds_int_array_push(arr, var1) == DS_STATUS_OK);
    assert(ds_int_array_push(arr, var2) == DS_STATUS_OK);
    assert(ds_int_array_push(arr, var3) == DS_STATUS_OK);
    assert(arr->length == 3);
    assert(ds_int_array_get(arr, 2) == 3);
    ds_int_array_destroy(arr);
    printf("test_array_growth_from_small_capacity passed\n");
}

static void test_array_shrink_floor() {
    size_t initial = (size_t)(DS_ARRAY_MIN_CAPACITY * 4);
    ds_int_array* arr = ds_int_array_create(initial);
    int dummy = 0;
    for (size_t i = 0; i < initial; ++i) {
        assert(ds_int_array_push(arr, dummy) == DS_STATUS_OK);
    }
    while (arr->length > 0) {
        ds_int_array_pop(arr, NULL);
    }
    assert(arr->capacity >= DS_ARRAY_MIN_CAPACITY);
    ds_int_array_destroy(arr);
    printf("test_array_shrink_floor passed\n");
}

static void test_array_self_append_rejected() {
    ds_int_array* arr = ds_int_array_create(4);
    int var = 1;
    ds_int_array_push(arr, var);
    assert(ds_int_array_push_array(arr, arr) == DS_STATUS_ERROR);
    ds_int_array_destroy(arr);
    printf("test_array_self_append_rejected passed\n");
}

static void test_array_destroy_with() {
    ds_int_array* arr = ds_int_array_create(4);
    ds_int_array_push(arr, 5);
    ds_int_array_push(arr, 10);
    ds_int_array_destroy(arr);
    printf("test_array_destroy_with passed\n");
}

static void test_array_out_of_bounds() {
    ds_int_array* arr = ds_int_array_create(4);
    int v = 1;
    assert(ds_int_array_get(arr, 0) == 0);
    assert(ds_int_array_get(arr, 100) == 0);
    assert(ds_int_array_remove(arr, 0, NULL) == DS_STATUS_OUT_OF_BOUNDS);
    assert(ds_int_array_pop(arr, NULL) == DS_STATUS_OUT_OF_BOUNDS);
    assert(ds_int_array_insert(arr, 100, v) == DS_STATUS_OUT_OF_BOUNDS);
    ds_int_array_destroy(arr);
    printf("test_array_out_of_bounds passed\n");
}

static void test_array_null_safety() {
    int v = 1;
    assert(ds_int_array_push(NULL, v) == DS_STATUS_IS_NULL);
    // assert(ds_int_array_get(NULL, 0) == 0);
    assert(ds_int_array_contains(NULL, v, NULL) == false);
    assert(ds_int_array_resize(NULL, 10) == DS_STATUS_IS_NULL);
    assert(ds_int_array_remove(NULL, 0, NULL) == DS_STATUS_IS_NULL);
    assert(ds_int_array_pop(NULL, NULL) == DS_STATUS_IS_NULL);
    ds_int_array_destroy(NULL);  // should not crash
    printf("test_array_null_safety passed\n");
}

static void test_array_shallow_copy() {
    ds_int_array* arr = ds_int_array_create(4);
    int var = 5;
    ds_int_array_push(arr, var);
    ds_int_array* clone = ds_int_array_shallow_copy(arr);
    assert(clone != NULL);
    assert(clone->length == arr->length);
    assert(ds_int_array_get(clone, 0) == ds_int_array_get(arr, 0));
    ds_int_array_destroy(arr);
    ds_int_array_destroy(clone);
    printf("test_array_shallow_copy passed\n");
}

static void test_array_reverse() {
    ds_int_array* arr = ds_int_array_create(5);
    int values[] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; ++i) {
        ds_int_array_push(arr, values[i]);
    }
    assert(ds_int_array_reverse(arr) == DS_STATUS_OK);
    for (int i = 0; i < 5; ++i) {
        int val = ds_int_array_get(arr, i);
        assert(val != 0);
        assert(val == values[4 - i]);
    }
    ds_int_array_destroy(arr);
    printf("test_array_reverse passed\n");
}

int main() {
    test_array_basic();
    test_array_growth_from_small_capacity();
    test_array_shrink_floor();
    test_array_self_append_rejected();
    test_array_destroy_with();
    // test_array_out_of_bounds();
    test_array_null_safety();
    test_array_shallow_copy();
    test_array_reverse();
    printf("All tests passed\n");
    return 0;
}