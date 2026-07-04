/**
 * @file ds_array.h
 * @author Valerius Petrini (valeriuspetrini@gmail.com)
 * @brief Implementation of a generic dynamic array in C, using `void*` to hold data of any type.
 * @version 0.1
 * @date 2026-07-04
 * 
 * @copyright Copyright (c) 2026
 */

#pragma once

#include "ds_common.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DS_ARRAY_FIELDS       \
    void* data;               \
    size_t length;            \
    size_t capacity;          \
    size_t member_size;       \
    ds_cmp_fn cmp_fn;         \
    ds_destroy_fn destroy_fn; \

/**
 * @brief A generic dynamic array, holding data using a `void*`
 */
typedef struct {
    DS_ARRAY_FIELDS
} ds_array;

// ----- FUNCTION DEFINITIONS -----
ds_array* ds_array_create(size_t initial_capacity, size_t member_size, ds_cmp_fn cmp_fn, ds_destroy_fn destroy_fn);
DS_STATUS ds_array_init(ds_array* array, size_t initial_capacity, size_t member_size, ds_cmp_fn cmp_fn, ds_destroy_fn destroy_fn);
DS_STATUS ds_array_init_default(ds_array* array, size_t member_size);
void ds_array_deinit(ds_array* array);
void ds_array_destroy(ds_array* array);
DS_STATUS ds_array_resize(ds_array* array, size_t new_capacity);

DS_STATUS ds_array_push_array(ds_array* array, const ds_array* other_array);
DS_STATUS ds_array_push(ds_array* array, const void* element);
DS_STATUS ds_array_insert(ds_array* array, size_t index, const void* element);
void* ds_array_get(ds_array* array, size_t index);
const void* ds_array_get_const(const ds_array* array, size_t index);
DS_STATUS ds_array_remove(ds_array* array, size_t index, void* out);
DS_STATUS ds_array_pop(ds_array* array, void* out);
bool ds_array_contains(const ds_array* array, const void* element);
void ds_array_clear(ds_array* array);
ds_array* ds_array_clone(const ds_array* array);
DS_STATUS ds_array_reverse(ds_array* array);

/**
 * @brief Defines a typed array struct and all its functions.
 * @param T    The element type (e.g. int, Token).
 * @param name The name prefix for the generated functions.
 *
 * @code
 * DEFINE_DS_ARRAY(int, ds_int_array)
 * ds_int_array arr;
 * ds_int_array_init(&arr, 8, NULL, NULL);
 * ds_int_array_push(&arr, 42);
 * @endcode
 */
#define DS_DEFINE_ARRAY(T, name) \
    typedef struct { struct { DS_ARRAY_FIELDS }; } name; \
    \
    static inline name* name##_create(size_t initial_capacity, ds_cmp_fn cmp_fn, ds_destroy_fn destroy_fn) { \
        return (name*)ds_array_create(initial_capacity, sizeof(T), cmp_fn, destroy_fn); \
    } \
    \
    static inline DS_STATUS name##_init(name* array, size_t initial_capacity, ds_cmp_fn cmp_fn, ds_destroy_fn destroy_fn) { \
        return ds_array_init((ds_array*)array, initial_capacity, sizeof(T), cmp_fn, destroy_fn); \
    } \
    \
    static inline DS_STATUS name##_init_default(name* array) { \
        return ds_array_init_default((ds_array*)array, sizeof(T)); \
    } \
    \
    static inline void name##_deinit(name* array) { \
        ds_array_deinit((ds_array*)array); \
    } \
    \
    static inline void name##_destroy(name* array) { \
        ds_array_destroy((ds_array*)array); \
    } \
    static inline DS_STATUS name##_resize(name* array, size_t new_capacity) { \
        return ds_array_resize((ds_array*)array, new_capacity); \
    } \
    \
    static inline DS_STATUS name##_push(name* array, T value) { \
        return ds_array_push((ds_array*)array, &value); \
    } \
    \
    static inline DS_STATUS name##_push_array(name* array, const name* other_array) { \
        return ds_array_push_array((ds_array*)array, (const ds_array*)other_array); \
    } \
    \
    static inline DS_STATUS name##_insert(name* array, size_t index, T value) { \
        return ds_array_insert((ds_array*)array, index, &value); \
    } \
    \
    static inline T name##_get(name* array, size_t index) { \
        assert(array != NULL); \
        assert(index < ((ds_array*)array)->length); \
        return (*(T*)ds_array_get((ds_array*)array, index)); \
    } \
    \
    static inline const T name##_get_const(const name* array, size_t index) { \
        assert(array != NULL); \
        assert(index < ((ds_array*)array)->length); \
        return (*(const T*)ds_array_get_const((ds_array*)array, index)); \
    } \
    \
    static inline DS_STATUS name##_remove(name* array, size_t index, T* out) { \
        return ds_array_remove((ds_array*)array, index, out); \
    } \
    \
    static inline DS_STATUS name##_pop(name* array, T* out) { \
        return ds_array_pop((ds_array*)array, out); \
    } \
    \
    static inline bool name##_contains(const name* array, T value) { \
        return ds_array_contains((ds_array*)array, &value); \
    } \
    \
    static inline void name##_clear(name* array) { \
        ds_array_clear((ds_array*)array); \
    } \
    \
    static inline name* name##_clone(const name* array) { \
        return (name*)ds_array_clone((ds_array*)array); \
    } \
    \
    static inline DS_STATUS name##_reverse(name* array) { \
        return ds_array_reverse((ds_array*)array); \
    }

#define DS_ARRAY_MIN_CAPACITY 8

#ifdef DS_C_IMPLEMENTATION

static size_t ds_array_grow(size_t capacity) {
    return capacity < DS_ARRAY_MIN_CAPACITY ? DS_ARRAY_MIN_CAPACITY : capacity * 2;
}

// Implementation of ds_array functions
ds_array* ds_array_create(size_t initial_capacity, size_t member_size, ds_cmp_fn cmp_fn, ds_destroy_fn destroy_fn) {
    ds_array* array = (ds_array*)malloc(sizeof(ds_array));
    if (!array) {
        DS_HANDLE_FAILURE("malloc for array failed", NULL);
    }
    if (ds_array_init(array, initial_capacity, member_size, cmp_fn, destroy_fn) != DS_STATUS_OK) {
        free(array);
        DS_HANDLE_FAILURE("ds_array_init failed", NULL);
    }
    return array;
}

DS_STATUS ds_array_init(ds_array* array, size_t initial_capacity, size_t member_size, ds_cmp_fn cmp_fn, ds_destroy_fn destroy_fn) {
    if (!array) {
        DS_HANDLE_FAILURE("array is NULL", DS_STATUS_IS_NULL);
    }
    if (member_size == 0) {
        DS_HANDLE_FAILURE("member size is 0", DS_STATUS_MEMBER_SIZE_0);
    }
    if (initial_capacity == 0) {
        DS_HANDLE_FAILURE("initial capacity is 0", DS_STATUS_SIZE_0);
    }
    if (initial_capacity > SIZE_MAX / member_size) {
        DS_HANDLE_FAILURE("initial capacity too large", DS_STATUS_OVERFLOW);
    }
    array->data = malloc(initial_capacity * member_size);
    if (!array->data) {
        DS_HANDLE_FAILURE("array data initialization failed", DS_STATUS_ALLOC_FAIL);
    }
    array->member_size = member_size;
    array->length = 0;
    array->capacity = initial_capacity;
    array->cmp_fn = cmp_fn;
    array->destroy_fn = destroy_fn;
    return DS_STATUS_OK;
}

DS_STATUS ds_array_init_default(ds_array* array, size_t member_size) {
    return ds_array_init(array, DS_ARRAY_MIN_CAPACITY, member_size, NULL, NULL);
}

void ds_array_deinit(ds_array* array) {
    if (array) {
        if (array->destroy_fn) {
            for (size_t i = 0; i < array->length; i++) {
                void* item = (char*)array->data + (i * array->member_size);
                array->destroy_fn(item);
            }
        }
        free(array->data);
        array->data = NULL;
        array->length = 0;
        array->capacity = 0;
    }
}

void ds_array_destroy(ds_array* array) {
    if (array) {
        ds_array_deinit(array);        
        free(array);
    }
}

DS_STATUS ds_array_resize(ds_array* array, size_t new_capacity) {
    if (!array) {
        DS_HANDLE_FAILURE("array is NULL", DS_STATUS_IS_NULL);
    }
    if (new_capacity > SIZE_MAX / array->member_size) {
        DS_HANDLE_FAILURE("new capacity too large", DS_STATUS_OVERFLOW);
    }
    if (new_capacity < array->length && array->destroy_fn) {
        for (size_t i = new_capacity; i < array->length; i++) {
            void* item = (char*)array->data + (i * array->member_size);
            array->destroy_fn(item);
        }
    }
    void* new_data = realloc(array->data, new_capacity * array->member_size);
    if (!new_data) {
        DS_HANDLE_FAILURE("array data resize failed", DS_STATUS_ALLOC_FAIL);
    }
    array->data = new_data;
    array->capacity = new_capacity;
    if (array->length > new_capacity) {
        array->length = new_capacity;
    }
    return DS_STATUS_OK;
}

DS_STATUS ds_array_push_array(ds_array* array, const ds_array* other_array) {
    if (!array || !other_array) {
        DS_HANDLE_FAILURE("array or other_array is NULL", DS_STATUS_IS_NULL);
    }
    if (other_array->length == 0) {
        return DS_STATUS_OK;
    }
    if (array->member_size != other_array->member_size) {
        DS_HANDLE_FAILURE("member sizes do not match", DS_STATUS_MEMBER_SIZE_MISMATCH);
    }
    if (array == other_array) {
        DS_HANDLE_FAILURE("array and other_array are the same", DS_STATUS_ERROR);
    }

    size_t new_total_length = array->length + other_array->length;
    size_t needed = new_total_length;
    if (needed > array->capacity) {
        size_t new_cap = ds_array_grow(array->capacity);
        while (new_cap < needed) {
            new_cap = ds_array_grow(new_cap);
        }
        DS_STATUS resize_status = ds_array_resize(array, new_cap);
        if (resize_status != DS_STATUS_OK) {
            return resize_status;
        }
    }

    memcpy((char*)array->data + (array->length * array->member_size), 
           other_array->data, 
           other_array->length * array->member_size);

    array->length = new_total_length;
    return DS_STATUS_OK;
}

DS_STATUS ds_array_push(ds_array* array, const void* element) {
    if (!array || !element) {
        DS_HANDLE_FAILURE("array or element is NULL", DS_STATUS_IS_NULL);
    }
    if (array->length >= array->capacity) {
        DS_STATUS status = ds_array_resize(array, ds_array_grow(array->capacity));
        if (status != DS_STATUS_OK) {
            return status;
        }
    }
    memcpy((char*)array->data + (array->length * array->member_size), element, array->member_size);
    array->length++;
    return DS_STATUS_OK;
}

DS_STATUS ds_array_insert(ds_array* array, size_t index, const void* element) {
    if (!array || !element) {
        DS_HANDLE_FAILURE("array or element is NULL", DS_STATUS_IS_NULL);
    }
    if (index > array->length) {
        DS_HANDLE_FAILURE("index is out of bounds", DS_STATUS_OUT_OF_BOUNDS);
    }
    if (array->length >= array->capacity) {
        DS_STATUS status = ds_array_resize(array, ds_array_grow(array->capacity));
        if (status != DS_STATUS_OK) {
            return status;
        }
    }
    memmove(
        (char*)array->data + ((index + 1) * array->member_size), 
        (char*)array->data + (index * array->member_size), 
        (array->length - index) * 
        array->member_size);
    memcpy((char*)array->data + (index * array->member_size), element, array->member_size);
    array->length++;
    return DS_STATUS_OK;
}

void* ds_array_get(ds_array* array, size_t index) {
    if (!array || index >= array->length) {
        return NULL;
    }
    return (char*)array->data + (index * array->member_size);
}

const void* ds_array_get_const(const ds_array* array, size_t index) {
    if (!array || index >= array->length) {
        return NULL;
    }
    return (const char*)array->data + (index * array->member_size);
}

DS_STATUS ds_array_remove(ds_array* array, size_t index, void* out) {
    if (!array) {
        DS_HANDLE_FAILURE("array is NULL", DS_STATUS_IS_NULL);
    }
    if (index >= array->length) {
        DS_HANDLE_FAILURE("index is out of bounds", DS_STATUS_OUT_OF_BOUNDS);
    }
    void* removed_element = (char*)array->data + (index * array->member_size);
    if (out) {
        memcpy(out, removed_element, array->member_size);
    } else if (array->destroy_fn) {
        array->destroy_fn(removed_element);
    }
    memmove(
        (char*)array->data + (index * array->member_size), 
        (char*)array->data + ((index + 1) * array->member_size), 
        (array->length - index - 1) * array->member_size);
    array->length--;
    if (array->length > 0 &&
        array->length < array->capacity / 4 &&
        array->capacity / 2 >= DS_ARRAY_MIN_CAPACITY) {
        DS_STATUS status = ds_array_resize(array, array->capacity / 2);
        if (status != DS_STATUS_OK) {
            return status;
        }
    }
    return DS_STATUS_OK;
}

DS_STATUS ds_array_pop(ds_array* array, void* out) {
    if (!array) {
        DS_HANDLE_FAILURE("array is NULL", DS_STATUS_IS_NULL);
    }
    if (array->length == 0) {
        DS_HANDLE_FAILURE("array is empty", DS_STATUS_OUT_OF_BOUNDS);
    }
    void* popped_element = (char*)array->data + ((array->length - 1) * array->member_size);
    if (out) {
        memcpy(out, popped_element, array->member_size);
    } else if (array->destroy_fn) {
        array->destroy_fn(popped_element);
    }
    array->length--;
    if (array->length > 0 &&
        array->length < array->capacity / 4 &&
        array->capacity / 2 >= DS_ARRAY_MIN_CAPACITY) {
        DS_STATUS status = ds_array_resize(array, array->capacity / 2);
        if (status != DS_STATUS_OK) {
            return status;
        }
    }
    return true;
}

bool ds_array_contains(const ds_array* array, const void* element) {
    if (!array || !element) {
        return false;
    }

    for (size_t i = 0; i < array->length; ++i) {
        void* current = (char*)array->data + (i * array->member_size);

        if (array->cmp_fn) {
            if (array->cmp_fn(current, element) == 0) {
                return true;
            }
        } else {
            if (memcmp(current, element, array->member_size) == 0) {
                return true;
            }
        }
    }
    return false;
}

void ds_array_clear(ds_array* array) {
    if (array) {
        if (array->destroy_fn) {
            for (size_t i = 0; i < array->length; ++i) {
                void* item = (char*)array->data + (i * array->member_size);
                array->destroy_fn(item);
            }
        }
        array->length = 0;
    }
}

ds_array* ds_array_clone(const ds_array* array) {
    if (!array) {
        DS_HANDLE_FAILURE("array is NULL", NULL);
    }
    ds_array* new_array = ds_array_create(array->capacity, array->member_size, array->cmp_fn, array->destroy_fn);
    if (!new_array) {
        DS_HANDLE_FAILURE("failed to create new array", NULL);
    }
    memcpy(new_array->data, array->data, array->length * array->member_size);
    new_array->length = array->length;
    return new_array;
}

DS_STATUS ds_array_reverse(ds_array* array) {
    if (!array) {
        DS_HANDLE_FAILURE("array is NULL", DS_STATUS_IS_NULL);
    }
    if (array->length < 2) {
        return DS_STATUS_OK;
    }
    void* temp = malloc(array->member_size);
    if (!temp) {
        DS_HANDLE_FAILURE("failed to allocate memory", DS_STATUS_ALLOC_FAIL);
    }
    for (size_t i = 0; i < array->length / 2; ++i) {
        memcpy(temp, (char*)array->data + (i * array->member_size), array->member_size);
        memcpy((char*)array->data + (i * array->member_size), (char*)array->data + ((array->length - 1 - i) * array->member_size), array->member_size);
        memcpy((char*)array->data + ((array->length - 1 - i) * array->member_size), temp, array->member_size);
    }
    free(temp);
    return DS_STATUS_OK;
}
#endif