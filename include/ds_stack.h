/**
 * @file ds_stack.h
 * @brief Implementation of a generic stack in C, wrapping a dynamic array.
 */

/**
 * @def DS_DEFINE_STACK_IMPLEMENTATION
 * @brief Define this in exactly one .c file before including the header
 *        to enable the implementation.
 *
 * @par Usage
 * Only one translation unit should define this. Defining it in multiple
 * .c files will cause linker errors due to duplicate symbols.
 *
 * @code
 * // my_file.c
 * #define DS_DEFINE_STACK_IMPLEMENTATION
 * #include "ds_stack.h"
 * @endcode
 *
 * @warning Defining this in a header file will cause duplicate symbol
 *          errors at link time.
 */
#ifdef DOXYGEN
    #define DS_DEFINE_STACK_IMPLEMENTATION
#endif

#pragma once

#ifdef DS_DEFINE_STACK_IMPLEMETATION
#include "ds_array.h"
#include "ds_common.h"

typedef ds_array ds_stack;

#define DS_DEFINE_STACK(T, name) \
    typedef struct { struct { DS_ARRAY_FIELDS }; } name; \
    \
    static inline name* name##_create(size_t initial_capacity, ds_cmp_fn cmp_fn, ds_destroy_fn destroy_fn) { \
        return (name*)ds_array_create(initial_capacity, sizeof(T), cmp_fn, destroy_fn); \
    } \
    \
    static inline DS_STATUS name##_init(name* stack, size_t initial_capacity, ds_cmp_fn cmp_fn, ds_destroy_fn destroy_fn) { \
        return ds_array_init((ds_array*)stack, initial_capacity, sizeof(T), cmp_fn, destroy_fn); \
    } \
    \
    static inline DS_STATUS name##_init_default(name* stack) { \
        return ds_array_init_default((ds_array*)stack, sizeof(T)); \
    } \
    \
    static inline void name##_deinit(name* stack) { \
        ds_array_deinit((ds_array*)stack); \
    } \
    \
    static inline void name##_destroy(name* stack) { \
        ds_array_destroy((ds_array*)stack); \
    } \
    \
    static inline DS_STATUS name##_push(name* stack, T value) { \
        return ds_array_push((ds_array*)stack, &value); \
    } \
    \
    static inline DS_STATUS name##_pop(name* stack, T* out) { \
        return ds_array_pop((ds_array*)stack, out); \
    } \
    \
    static inline T name##_peek(name* stack) { \
        if (!stack || stack->length == 0) { \
            return (T){0}; /* Return a default value for the type */\
        }\
        return (*(T*)ds_array_get((ds_array*)stack, stack->length - 1));\
    }\

static inline ds_stack* ds_stack_create(size_t initial_capacity, size_t member_size, ds_cmp_fn cmp_fn, ds_destroy_fn destroy_fn) {
    return ds_array_create(initial_capacity, member_size, cmp_fn, destroy_fn);
}

static inline DS_STATUS ds_stack_init(ds_stack* stack, size_t initial_capacity, size_t member_size, ds_cmp_fn cmp_fn, ds_destroy_fn destroy_fn) {
    return ds_array_init(stack, initial_capacity, member_size, cmp_fn, destroy_fn);
}

static inline DS_STATUS ds_stack_init_default(ds_stack* stack, size_t member_size) {
    return ds_array_init_default(stack, member_size);
}

static inline void ds_stack_deinit(ds_stack* stack) {
    ds_array_deinit(stack);
}

static inline void ds_stack_destroy(ds_stack* stack) {
    ds_array_destroy(stack);
}

static inline DS_STATUS ds_stack_push(ds_stack* stack, const void* element) {
    return ds_array_push(stack, element);
}

static inline DS_STATUS ds_stack_pop(ds_stack* stack, void* out) {
    return ds_array_pop(stack, out);
}

static inline void* ds_stack_peek(ds_stack* stack) {
    if (!stack || stack->length == 0) {
        return NULL;
    }
    return ds_array_get(stack, stack->length - 1);
}

static inline DS_STATUS ds_stack_reverse(ds_stack* stack) {
    return ds_array_reverse(stack);
}
#endif