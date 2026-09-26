/**
 * @file ds_string.h
 * @brief Implementation of a dynamic string in C, using `char*` to hold data.
 */

/**
 * @defgroup string_api Dynamic String API
 * @brief Core dynamic string operations.
 */

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "ds_common.h"

/**
 * @brief A dynamic string, holding data using a `char*`
 * @ingroup string_api
 */
typedef struct {
    char* data; /**< Pointer to the internal buffer. */
    size_t
        length; /**< Number of characters currently stored (excluding NUL). */
    size_t capacity; /**< Total allocated capacity in characters (including
                        NUL). */
} ds_string;

// ----- FUNCTION DEFINITIONS -----

/**
 * @brief Creates a new dynamic string with the specified initial data.
 *
 * @param initial_data The initial string data to copy into the new string. Can
 * be NULL for an empty string.
 *
 * @return A pointer to the newly created string
 * @return NULL if memory allocation fails.
 *
 * @note The caller is responsible for freeing the returned string using
 * ds_string_destroy().
 *
 * @see ds_string_destroy
 * @see ds_string_init
 *
 * @memberof ds_string
 * @ingroup string_api
 */
ds_string* ds_string_create(const char* initial_data);
/**
 * @brief Initializes an existing dynamic string with the specified initial
 * data.
 *
 * @param str The string to initialize. Must not be NULL.
 * @param initial_data The initial string data to copy into the string. Can be
 * NULL for an empty string.
 *
 * @return DS_STATUS_OK on success
 * @return DS_STATUS_IS_NULL if str is NULL
 * @return DS_STATUS_ALLOC_FAIL if internal buffer allocation fails
 * @return DS_STATUS_OVERFLOW if the initial data is too large to fit in the
 * buffer
 *
 * @note Use @ref ds_string_deinit to free the internal buffer when done.
 *
 * @see ds_string_deinit
 * @see ds_string_destroy
 *
 * @memberof ds_string
 * @ingroup string_api
 */
DS_STATUS ds_string_init(ds_string* str, const char* initial_data);
/**
 * @brief Deinitializes an existing dynamic string, freeing its internal buffer
 * but not the string struct itself.
 *
 * @param str The string to deinitialize. Must not be NULL.
 *
 * @note Use @ref ds_string_destroy to free the string struct itself.
 *
 * @see ds_string_destroy
 * @see ds_string_init
 *
 * @memberof ds_string
 * @ingroup string_api
 */
void ds_string_deinit(ds_string* str);
/**
 * @brief Destroys a dynamic string, freeing both its internal buffer and the
 * string struct itself.
 *
 * @param str The string to destroy. Must not be NULL.
 *
 * @note Use @ref ds_string_deinit if you only want to free the internal buffer.
 *
 * @see ds_string_deinit
 * @see ds_string_init
 *
 * @memberof ds_string
 * @ingroup string_api
 */
void ds_string_destroy(ds_string* str);
/**
 * @brief Resizes the internal buffer of a dynamic string to a new capacity.
 *
 * @param str The string to resize. Must not be NULL.
 * @param new_capacity The new capacity for the string, including space for the
 * NUL terminator. Must be greater than 0.
 *
 * @return DS_STATUS_OK on success
 * @return DS_STATUS_IS_NULL if str is NULL
 * @return DS_STATUS_SIZE_0 if new_capacity is 0
 * @return DS_STATUS_ALLOC_FAIL if internal buffer allocation fails
 *
 * @note If the new capacity is smaller than the current length of the string,
 * the string will be truncated to fit within the new capacity.
 *
 * @see ds_string_init
 * @see ds_string_deinit
 *
 * @memberof ds_string
 * @ingroup string_api
 */
DS_STATUS ds_string_resize(ds_string* str, size_t new_capacity);

/**
 * @brief Appends a single character to the end of a dynamic string.
 *
 * @param str The string to which the character will be appended. Must not be
 * NULL.
 * @param suffix The character to append.
 *
 * @return DS_STATUS_OK on success
 * @return DS_STATUS_IS_NULL if str is NULL
 * @return DS_STATUS_ALLOC_FAIL if internal buffer allocation fails
 *
 * @note If the string's length is equal to or greater than its capacity, the
 * internal buffer will be resized to accommodate the new character.
 *
 * @see ds_string_append_c_str
 * @see ds_string_append
 *
 * @memberof ds_string
 * @ingroup string_api
 */
DS_STATUS ds_string_append_char(ds_string* str, char suffix);
/**
 * @brief Appends a C-style string to the end of a dynamic string.
 *
 * @param str The string to which the C-style string will be appended. Must not
 * be NULL.
 * @param suffix The C-style string to append. Must not be NULL.
 *
 * @return DS_STATUS_OK on success
 * @return DS_STATUS_IS_NULL if str or suffix is NULL
 * @return DS_STATUS_ALLOC_FAIL if internal buffer allocation fails
 * @return DS_STATUS_OVERFLOW if the resulting string would exceed `SIZE_MAX`
 *
 * @note If the string's length plus the length of the suffix is equal to or
 * greater than its capacity, the internal buffer will be resized to
 * accommodate the new data.
 *
 * @see ds_string_append_char
 * @see ds_string_append
 *
 * @memberof ds_string
 * @ingroup string_api
 */
DS_STATUS ds_string_append_c_str(ds_string* str, const char* suffix);
/**
 * @brief Appends another dynamic string to the end of a dynamic string.
 *
 * @param str The string to which the other string will be appended. Must not be
 * NULL.
 * @param suffix The dynamic string to append. Must not be NULL.
 *
 * @return DS_STATUS_OK on success
 * @return DS_STATUS_IS_NULL if str or suffix is NULL
 * @return DS_STATUS_ALLOC_FAIL if internal buffer allocation fails
 * @return DS_STATUS_OVERFLOW if the resulting string would exceed `SIZE_MAX`
 *
 * @note If the string's length plus the length of the suffix is equal to or
 * greater than its capacity, the internal buffer will be resized to
 * accommodate the new data.
 *
 * @see ds_string_append_char
 * @see ds_string_append_c_str
 *
 * @memberof ds_string
 * @ingroup string_api
 */
DS_STATUS ds_string_append(ds_string* str, const ds_string* suffix);
/**
 * @brief Compares two dynamic strings for equality.
 *
 * @param str The first string to compare. Must not be NULL.
 * @param other The second string to compare. Must not be NULL.
 *
 * @return true if the strings are equal, false otherwise.
 *
 * @see ds_string_equals_cstr
 *
 * @memberof ds_string
 * @ingroup string_api
 */
bool ds_string_equals(const ds_string* str, const ds_string* other);
/**
 * @brief Compares a dynamic string with a C-style string for equality.
 *
 * @param str The dynamic string to compare. Must not be NULL.
 * @param cstr The C-style string to compare. Must not be NULL.
 *
 * @return true if the strings are equal, false otherwise.
 *
 * @see ds_string_equals
 *
 * @memberof ds_string
 * @ingroup string_api
 */
bool ds_string_equals_cstr(const ds_string* str, const char* cstr);
/**
 * @brief Returns a pointer to the internal C-style string of a dynamic string.
 *
 * @param str The dynamic string. Must not be NULL.
 *
 * @return A pointer to the internal C-style string, or NULL if str is NULL.
 *
 * @note The returned pointer is valid as long as the dynamic string is not
 * modified or destroyed.
 *
 * @see ds_string_init
 * @see ds_string_deinit
 *
 * @memberof ds_string
 * @ingroup string_api
 */
const char* ds_string_c_str(const ds_string* str);
/**
 * @brief Clears the contents of a dynamic string, resetting its length to 0 and
 * setting the first character to NUL.
 *
 * @param str The dynamic string to clear. Must not be NULL.
 *
 * @note The capacity of the string remains unchanged.
 *
 * @see ds_string_init
 * @see ds_string_deinit
 *
 * @memberof ds_string
 * @ingroup string_api
 */
void ds_string_clear(ds_string* str);

#ifdef DS_C_IMPLEMENTATION

#define DS_STRING_MIN_CAPACITY 16

ds_string* ds_string_create(const char* initial_data) {
    ds_string* str = (ds_string*)malloc(sizeof(ds_string));
    if (!str) {
        DS_HANDLE_FAILURE("Failed to allocate memory for ds_string", NULL);
    }
    if (ds_string_init(str, initial_data) != DS_STATUS_OK) {
        free(str);
        DS_HANDLE_FAILURE("Failed to initialize ds_string", NULL);
    }
    return str;
}

DS_STATUS ds_string_init(ds_string* str, const char* initial_data) {
    if (!str) {
        DS_HANDLE_FAILURE("String is NULL", DS_STATUS_IS_NULL);
    }
    size_t initial_length = initial_data ? strlen(initial_data) : 0;
    if (initial_length == SIZE_MAX) {
        DS_HANDLE_FAILURE("String too large", DS_STATUS_OVERFLOW);
    }
    size_t initial_capacity = initial_length + 1;

    if (initial_capacity < DS_STRING_MIN_CAPACITY) {
        initial_capacity = DS_STRING_MIN_CAPACITY;
    }

    str->data = (char*)malloc(initial_capacity);
    if (!str->data) {
        DS_HANDLE_FAILURE("Failed to allocate memory for ds_string data",
                          DS_STATUS_ALLOC_FAIL);
    }
    if (initial_data) {
        memcpy(str->data, initial_data, initial_length);
    }
    str->data[initial_length] = '\0';
    str->length = initial_length;
    str->capacity = initial_capacity;
    return DS_STATUS_OK;
}

void ds_string_deinit(ds_string* str) {
    if (str) {
        free(str->data);
        str->data = NULL;
        str->length = 0;
        str->capacity = 0;
    }
}

void ds_string_destroy(ds_string* str) {
    if (str) {
        free(str->data);
        free(str);
    }
}

DS_STATUS ds_string_resize(ds_string* str, size_t new_capacity) {
    if (!str) {
        DS_HANDLE_FAILURE("str is NULL", DS_STATUS_IS_NULL);
    }
    if (new_capacity == 0) {
        DS_HANDLE_FAILURE("new_capacity is 0", DS_STATUS_SIZE_0);
    }
    char* new_data = (char*)realloc(str->data, new_capacity);
    if (!new_data) {
        DS_HANDLE_FAILURE("realloc for string data failed",
                          DS_STATUS_ALLOC_FAIL);
    }
    str->data = new_data;
    str->capacity = new_capacity;
    if (str->length >= new_capacity) {
        str->length = new_capacity - 1;  // truncate, leaving room for NUL
    }
    str->data[str->length] = '\0';
    return DS_STATUS_OK;
}

DS_STATUS ds_string_append_char(ds_string* str, char suffix) {
    if (!str) {
        DS_HANDLE_FAILURE("String is NULL", DS_STATUS_IS_NULL);
    }
    if (str->length + 1 >= str->capacity) {
        size_t new_capacity = (str->capacity * 2) + 1;
        DS_STATUS result = ds_string_resize(str, new_capacity);
        if (result != DS_STATUS_OK) {
            return result;
        }
    }
    str->data[str->length] = suffix;
    str->length++;
    str->data[str->length] = '\0';
    return DS_STATUS_OK;
}

DS_STATUS ds_string_append_c_str(ds_string* str, const char* suffix) {
    if (!str || !suffix) {
        DS_HANDLE_FAILURE("String or suffix is NULL", DS_STATUS_IS_NULL);
    }
    size_t suffix_length = strlen(suffix);
    if (suffix_length > SIZE_MAX - str->length) {
        DS_HANDLE_FAILURE("String length too large", DS_STATUS_OVERFLOW);
    }
    size_t new_length = str->length + suffix_length;

    if (new_length + 1 > str->capacity) {
        size_t new_capacity = (new_length * 2) + 1;
        DS_STATUS result = ds_string_resize(str, new_capacity);
        if (result != DS_STATUS_OK) {
            return result;
        }
    }

    memcpy(str->data + str->length, suffix, suffix_length);
    str->length = new_length;
    str->data[str->length] = '\0';
    return DS_STATUS_OK;
}

DS_STATUS ds_string_append(ds_string* str, const ds_string* suffix) {
    if (!str || !suffix) {
        DS_HANDLE_FAILURE("String or suffix is NULL", DS_STATUS_IS_NULL);
    }
    return ds_string_append_c_str(str, suffix->data);
}

bool ds_string_equals(const ds_string* str, const ds_string* other) {
    if (!str || !other || str->length != other->length) {
        return false;
    }
    return memcmp(str->data, other->data, str->length) == 0;
}

bool ds_string_equals_cstr(const ds_string* str, const char* cstr) {
    if (!str || !cstr) {
        return false;
    }
    size_t cstr_length = strlen(cstr);
    if (str->length != cstr_length) {
        return false;
    }
    return memcmp(str->data, cstr, str->length) == 0;
}

const char* ds_string_c_str(const ds_string* str) {
    if (!str) {
        DS_HANDLE_FAILURE("String is NULL", NULL);
    }
    return str->data;
}

void ds_string_clear(ds_string* str) {
    if (str) {
        str->length = 0;
        if (str->data) {
            str->data[0] = '\0';
        }
    }
}

#endif
