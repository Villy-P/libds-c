#pragma once

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

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

bool ds_array_push_array(ds_array* array, ds_array* other_array);
bool ds_array_push(ds_array* array, void* element);
bool ds_array_insert(ds_array* array, size_t index, void* element);
void* ds_array_get(const ds_array* array, size_t index);
void* ds_array_remove(ds_array* array, size_t index);
void* ds_array_pop(ds_array* array);
bool ds_array_contains(ds_array* array, const void* element);
void ds_array_clear(ds_array* array);
ds_array* ds_array_clone(const ds_array* array);

#ifndef LIBDS_C_IGNORE_MACROS
    #define DSM_ARRAY_GET(array, index, type) \
        ((type)ds_array_get((array), (index)))
#endif

#ifdef DS_C_IMPLEMENTATION
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

bool ds_array_push_array(ds_array* array, ds_array* other_array) {
    if (!array || !other_array || other_array->length == 0) {
        return false;
    }

    size_t new_total_length = array->length + other_array->length;
    if (new_total_length > array->capacity) {
        if (!ds_array_resize(array, new_total_length)) {
            return false;
        }
    }

    memcpy((void*)&array->data[array->length], 
           (void*)other_array->data, 
           other_array->length * sizeof(void*));

    array->length = new_total_length;
    return true;
}

bool ds_array_push(ds_array* array, void* element) {
    if (!array) {
        return false;
    }
    if (array->length >= array->capacity) {
        if (!ds_array_resize(array, array->capacity * 2)) {
            return false;
        }
    }
    array->data[array->length++] = element;
    return true;
}

bool ds_array_insert(ds_array* array, size_t index, void* element) {
    if (!array || index > array->length) {
        return false;
    }
    if (array->length >= array->capacity) {
        size_t new_capacity = (array->capacity == 0) ? 1 : array->capacity * 2;
        if (!ds_array_resize(array, new_capacity)) {
            return false;
        }
    }
    memmove(
        (void*)&array->data[index + 1], 
        (const void*)&array->data[index], 
        (array->length - index) * 
        sizeof(void*));
    array->data[index] = element;
    array->length++;
    return true;
}

void* ds_array_get(const ds_array* array, size_t index) {
    if (!array || index >= array->length) {
        return NULL;
    }
    return array->data[index];
}

void* ds_array_remove(ds_array* array, size_t index) {
    if (!array || index >= array->length) {
        return NULL;
    }
    void* removed_element = array->data[index];
    memmove(
        (void*)&array->data[index], 
        (const void*)&array->data[index + 1], 
        (array->length - index - 1) * sizeof(void*));
    if (array->length > 0 && array->length < array->capacity / 4) {
        ds_array_resize(array, array->capacity / 2);
    }
    array->length--;
    return removed_element;
}

void* ds_array_pop(ds_array* array) {
    if (!array || array->length == 0) {
        return NULL;
    }
    void* popped_element = array->data[array->length - 1];
    array->length--;
    if (array->length > 0 && array->length < array->capacity / 4) {
        ds_array_resize(array, array->capacity / 2);
    }
    return popped_element;
}

bool ds_array_contains(ds_array* array, const void* element) {
    if (!array) {
        return false;
    }
    for (size_t i = 0; i < array->length; ++i) {
        if (array->data[i] == element) {
            return true;
        }
    }
    return false;
}

void ds_array_clear(ds_array* array) {
    if (array) {
        array->length = 0;
    }
}

ds_array* ds_array_clone(const ds_array* array) {
    if (!array) {
        return nullptr;
    }
    ds_array* new_array = ds_array_create(array->capacity);
    if (!new_array) {
        return nullptr;
    }
    memcpy((void*)new_array->data, (void*)array->data, array->length * sizeof(void*));
    new_array->length = array->length;
    return new_array;
}
#endif