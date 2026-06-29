#pragma once

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

#ifdef DS_PANIC_ON_MALLOC
    #define DS_HANDLE_MALLOC_FAILURE(msg) \
        do { \
            fprintf(stderr, "[LIB-DS-C FATAL] Out of memory: %s\n", msg); \
            exit(EXIT_FAILURE); \
        } while(0)
#else
    #define DS_HANDLE_MALLOC_FAILURE(msg) return nullptr
#endif

typedef struct {
    void** data;
    size_t length;
    size_t capacity;
} ds_array;


// ----- FUNCTION DEFINITIONS -----

// ----- ARRAY -----
ds_array* ds_array_create(size_t initial_capacity);
bool ds_array_init(ds_array* array, size_t initial_capacity);
void ds_array_destroy(ds_array* array);
bool ds_array_resize(ds_array* array, size_t new_capacity);

bool ds_array_push(ds_array* array, void* element);
bool ds_array_insert(ds_array* array, size_t index, void* element);
void* ds_array_get(ds_array* array, size_t index);
void* ds_array_remove(ds_array* array, size_t index);
void ds_array_clear(ds_array* array);
bool ds_array_contains(ds_array* array, void* element);

#ifdef LIBDS_C_IMPLEMENTATION
// Implementation of ds_array functions
ds_array* ds_array_create(size_t initial_capacity) {
    ds_array* array = (ds_array*)malloc(sizeof(ds_array));
    if (!array) {
        DS_HANDLE_MALLOC_FAILURE("Failed to allocate memory for ds_array");
    }
    if (!ds_array_init(array, initial_capacity)) {
        free(array);
        DS_HANDLE_MALLOC_FAILURE("Failed to initialize ds_array");
    }
    return array;
}

bool ds_array_init(ds_array* array, size_t initial_capacity) {
    assert(initial_capacity > 0);
    if (!array) {
        return false;
    }
    array->data = (void**)malloc(initial_capacity * sizeof(void*));
    if (!array->data) {
        return false;
    }
    array->length = 0;
    array->capacity = initial_capacity;
    return true;
}

void ds_array_destroy(ds_array* array) {
    if (array) {
        free((void*)array->data);
        free(array);
    }
}

bool ds_array_resize(ds_array* array, size_t new_capacity) {
    assert(new_capacity >= 0);
    if (!array) {
        return false;
    }
    void** new_data = (void**)realloc((void*)array->data, new_capacity * sizeof(void*));
    if (!new_data) {
        return false;
    }
    array->data = new_data;
    array->capacity = new_capacity;
    if (array->length > new_capacity) {
        array->length = new_capacity;
    }
    return true;
}
#endif