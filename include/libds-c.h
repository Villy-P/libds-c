#pragma once

#include <stddef.h>

typedef struct {
    void** data;
    size_t length;
    size_t capacity;
} ds_array;

ds_array* ds_array_create(size_t initial_capacity);
bool ds_array_init(ds_array* array, size_t initial_capacity);
void ds_array_destroy(ds_array* array);

bool ds_array_push(ds_array* array, void* element);
bool ds_array_insert(ds_array* array, size_t index, void* element);
void* ds_array_get(ds_array* array, size_t index);
void* ds_array_remove(ds_array* array, size_t index);
void ds_array_clear(ds_array* array);
bool ds_array_contains(ds_array* array, void* element);

bool ds_array_resize(ds_array* array, size_t new_capacity);

#ifdef LIBDS_C_IMPLEMENTATION

#endif