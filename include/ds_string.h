#pragma once

#include "ds_common.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char* data;
    size_t length;
    size_t capacity;
} ds_string;

// ----- FUNCTION DEFINITIONS -----
ds_string* ds_string_create(const char* initial_data);
bool ds_string_init(ds_string* str, const char* initial_data);
void ds_string_destroy(ds_string* str);
bool ds_string_resize(ds_string* str, size_t new_capacity);

#ifdef DS_C_IMPLEMENTATION

#define DS_STRING_MIN_CAPACITY 16
ds_string* ds_string_create(const char* initial_data) {
    ds_string* str = (ds_string*)malloc(sizeof(ds_string));
    if (!str) {
        DS_HANDLE_MALLOC_FAILURE("Failed to allocate memory for ds_string", NULL);
    }
    if (!ds_string_init(str, initial_data)) {
        free(str);
        DS_HANDLE_MALLOC_FAILURE("Failed to initialize ds_string", NULL);
    }
    return str;
}

bool ds_string_init(ds_string* str, const char* initial_data) {
    if (!str) {
        return false;
    }
    size_t initial_length = initial_data ? strlen(initial_data) : 0;
    if (initial_length == SIZE_MAX) {
        return false;
    }
    size_t initial_capacity = initial_length + 1;

    if (initial_capacity < DS_STRING_MIN_CAPACITY) {
        initial_capacity = DS_STRING_MIN_CAPACITY;
    } else {
        initial_capacity = (initial_length * 2) + 1;
    }

    str->data = (char*)malloc(initial_capacity);
    if (!str->data) {
        DS_HANDLE_MALLOC_FAILURE("Failed to allocate memory for ds_string data", false);
    }
    if (initial_data) {
        memcpy(str->data, initial_data, initial_length);
    }
    str->data[initial_length] = '\0';
    str->length = initial_length;
    str->capacity = initial_capacity;
    return true;
}
#endif