#define DS_C_IMPLEMENTATION
#include "libds_c.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static const int TEST_ARRAY_INITIAL_CAPACITY = 10;
static const int TEST_ARRAY_RESIZE_CAPACITY = 20;
static const int TEST_ARRAY_PUSH_COUNT = 15;
static const int TEST_ARRAY_INSERT_VALUE = 99;
static const int TEST_ARRAY_INSERT_INDEX = 5;

static void test_array_basic() {
    ds_array* arr = ds_array_create(TEST_ARRAY_INITIAL_CAPACITY);
    assert(arr != NULL);
    assert(arr->capacity == (size_t)TEST_ARRAY_INITIAL_CAPACITY);
    assert(arr->length == 0);

    assert(ds_array_resize(arr, TEST_ARRAY_RESIZE_CAPACITY) == true);
    assert(arr->capacity == (size_t)TEST_ARRAY_RESIZE_CAPACITY);

    int test_values[TEST_ARRAY_PUSH_COUNT];
    for (int i = 0; i < TEST_ARRAY_PUSH_COUNT; ++i) {
        test_values[i] = i;
        assert(ds_array_push(arr, &test_values[i]) == true);
        assert(arr->length == (size_t)(i + 1));
    }

    for (int i = 0; i < TEST_ARRAY_PUSH_COUNT; ++i) {
        int* retrieved = DSM_ARRAY_GET(arr, i, int*);
        assert(retrieved != NULL);
        assert(*retrieved == i);
    }

    int insert_value = TEST_ARRAY_INSERT_VALUE;
    assert(ds_array_insert(arr, TEST_ARRAY_INSERT_INDEX, &insert_value) == true);
    int* retrieved_insert = DSM_ARRAY_GET(arr, TEST_ARRAY_INSERT_INDEX, int*);
    assert(retrieved_insert != NULL);
    assert(*retrieved_insert == TEST_ARRAY_INSERT_VALUE);

    int* removed_element = ds_array_remove(arr, TEST_ARRAY_INSERT_INDEX);
    assert(removed_element != NULL);
    int* retrieved_after_remove = DSM_ARRAY_GET(arr, TEST_ARRAY_INSERT_INDEX, int*);
    assert(retrieved_after_remove != NULL);

    int* popped_value = ds_array_pop(arr);
    assert(popped_value != NULL);
    assert(*popped_value == 14);

    assert(ds_array_contains(arr, &test_values[7]) == true);
    int unrelated_value = 7;
    assert(ds_array_contains(arr, &unrelated_value) == false);

    ds_array_clear(arr);
    assert(arr->length == 0);

    ds_array_destroy(arr);
    printf("test_array_basic passed\n");
}

static void test_array_growth_from_small_capacity() {
    ds_array* arr = ds_array_create(1);
    int var1 = 1;
    int var2 = 2;
    int var3 = 3;
    assert(ds_array_push(arr, &var1) == true);
    assert(ds_array_push(arr, &var2) == true); 
    assert(ds_array_push(arr, &var3) == true);
    assert(arr->length == 3);
    assert(*(int*)ds_array_get(arr, 2) == 3);
    ds_array_destroy(arr);
    printf("test_array_growth_from_small_capacity passed\n");
}

static void test_array_shrink_floor() {
    size_t initial = (size_t)(DS_ARRAY_MIN_CAPACITY * 4);
    ds_array* arr = ds_array_create(initial);
    int dummy = 0;
    for (size_t i = 0; i < initial; ++i) {
        assert(ds_array_push(arr, &dummy) == true);
    }
    while (arr->length > 0) {
        ds_array_pop(arr);
    }
    assert(arr->capacity >= DS_ARRAY_MIN_CAPACITY);
    ds_array_destroy(arr);
    printf("test_array_shrink_floor passed\n");
}

static void test_array_self_append_rejected() {
    ds_array* arr = ds_array_create(4);
    int var = 1;
    ds_array_push(arr, &var);
    assert(ds_array_push_array(arr, arr) == false);
    ds_array_destroy(arr);
    printf("test_array_self_append_rejected passed\n");
}

static void test_array_destroy_with() {
    ds_array* arr = ds_array_create(4);
    ds_array_push(arr, malloc(sizeof(int)));
    ds_array_push(arr, malloc(sizeof(int)));
    ds_array_destroy_with(arr, free); // should not leak or crash
    printf("test_array_destroy_with passed\n");
}

static void test_array_out_of_bounds() {
    ds_array* arr = ds_array_create(4);
    int v = 1;
    assert(ds_array_get(arr, 0) == NULL);
    assert(ds_array_get(arr, 100) == NULL);
    assert(ds_array_remove(arr, 0) == NULL);
    assert(ds_array_pop(arr) == NULL);
    assert(ds_array_insert(arr, 100, &v) == false);
    ds_array_destroy(arr);
    printf("test_array_out_of_bounds passed\n");
}

static void test_array_null_safety() {
    int v = 1;
    assert(ds_array_push(NULL, &v) == false);
    assert(ds_array_get(NULL, 0) == NULL);
    assert(ds_array_contains(NULL, &v) == false);
    assert(ds_array_resize(NULL, 10) == false);
    assert(ds_array_remove(NULL, 0) == NULL);
    assert(ds_array_pop(NULL) == NULL);
    ds_array_destroy(NULL); // should not crash
    printf("test_array_null_safety passed\n");
}

static void test_array_clone() {
    ds_array* arr = ds_array_create(4);
    int var = 5;
    ds_array_push(arr, &var);
    ds_array* clone = ds_array_clone(arr);
    assert(clone != NULL);
    assert(clone->length == arr->length);
    assert(ds_array_get(clone, 0) == ds_array_get(arr, 0));
    ds_array_destroy(arr);
    ds_array_destroy(clone);
    printf("test_array_clone passed\n");
}

int main() {
    test_array_basic();
    test_array_growth_from_small_capacity();
    test_array_shrink_floor();
    test_array_self_append_rejected();
    test_array_destroy_with();
    test_array_out_of_bounds();
    test_array_null_safety();
    test_array_clone();
    printf("All tests passed\n");
    return 0;
}