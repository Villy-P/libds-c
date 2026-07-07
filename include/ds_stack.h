/**
 * @file ds_stack.h
 * @brief Implementation of a generic stack in C, wrapping a dynamic array.
 */

/**
 * @defgroup stack_api Stack API
 * @brief Core stack operations.
 */

#pragma once

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
 *
 * @ingroup stack_api
 */
#ifdef DOXYGEN
#define DS_DEFINE_STACK_IMPLEMENTATION
#endif

#ifdef DS_DEFINE_STACK_IMPLEMENTATION
#include "ds_array.h"
#include "ds_common.h"

/**
 * @brief A generic stack data structure, implemented as a wrapper around a
 * dynamic array.
 * @ingroup stack_api
 */
typedef ds_array ds_stack;

/**
 * @brief Defines a typed stack struct and all its functions.
 * @param T    The element type (e.g. int, Token).
 * @param name The name prefix for the generated functions.
 *
 * @code
 * DEFINE_DS_STACK(int, ds_int_stack)
 * ds_int_stack arr;
 * ds_int_stack_init(&arr, 8, NULL, NULL);
 * ds_int_stack_push(&arr, 42);
 * @endcode
 *
 * @ingroup stack_api
 */
#define DS_DEFINE_STACK(T, name)                                             \
    typedef struct {                                                         \
        struct {                                                             \
            DS_ARRAY_FIELDS                                                  \
        };                                                                   \
    } name;                                                                  \
                                                                             \
    static inline name* name##_create(                                       \
        size_t initial_capacity, ds_cmp_fn cmp_fn, ds_destroy_fn destroy_fn, \
        ds_copy_fn copy_fn) {                                                \
        return (name*)ds_array_create(initial_capacity, sizeof(T), cmp_fn,   \
                                      destroy_fn, copy_fn);                  \
    }                                                                        \
                                                                             \
    static inline DS_STATUS name##_init(                                     \
        name* stack, size_t initial_capacity, ds_cmp_fn cmp_fn,              \
        ds_destroy_fn destroy_fn, ds_copy_fn copy_fn) {                      \
        return ds_array_init((ds_array*)stack, initial_capacity, sizeof(T),  \
                             cmp_fn, destroy_fn, copy_fn);                   \
    }                                                                        \
                                                                             \
    static inline DS_STATUS name##_init_default(name* stack) {               \
        return ds_array_init_default((ds_array*)stack, sizeof(T));           \
    }                                                                        \
                                                                             \
    static inline void name##_deinit(name* stack) {                          \
        ds_array_deinit((ds_array*)stack);                                   \
    }                                                                        \
                                                                             \
    static inline void name##_destroy(name* stack) {                         \
        ds_array_destroy((ds_array*)stack);                                  \
    }                                                                        \
                                                                             \
    static inline DS_STATUS name##_push(name* stack, T value) {              \
        return ds_array_push((ds_array*)stack, &value);                      \
    }                                                                        \
                                                                             \
    static inline DS_STATUS name##_pop(name* stack, T* out) {                \
        return ds_array_pop((ds_array*)stack, out);                          \
    }                                                                        \
                                                                             \
    static inline T name##_peek(name* stack) {                               \
        if (!stack || stack->length == 0) {                                  \
            return (T){0}; /* Return a default value for the type */         \
        }                                                                    \
        return (*(T*)ds_array_get((ds_array*)stack, stack->length - 1));     \
    }

/**
 * @brief Creates a new stack with the specified initial capacity and element
 * size.
 *
 * @param initial_capacity The initial capacity of the stack.
 * @param member_size The size of each element in bytes.
 * @param cmp_fn Optional comparison function for elements, NULL if not needed.
 * @param destroy_fn Optional destructor function for elements, NULL if not
 * needed.
 * @param copy_fn Optional copy function for elements, NULL if not needed.
 *
 * @return A pointer to the newly created stack
 * @return NULL if memory allocation fails, or if initial_capacity or
 * member_size is 0.
 *
 * @note The caller is responsible for freeing the returned stack using
 * ds_stack_destroy().
 *
 * @see ds_stack_destroy
 * @see ds_stack_init
 * @see ds_array::ds_array_create
 *
 * @ingroup stack_api
 */
static inline ds_stack* ds_stack_create(size_t initial_capacity,
                                        size_t member_size, ds_cmp_fn cmp_fn,
                                        ds_destroy_fn destroy_fn,
                                        ds_copy_fn copy_fn) {
    return ds_array_create(initial_capacity, member_size, cmp_fn, destroy_fn,
                           copy_fn);
}

/**
 * @brief Initializes an existing stack with the specified initial capacity and
 * element size.
 *
 * @param stack The stack to initialize. Must not be NULL.
 * @param initial_capacity The initial capacity of the stack. Must be greater
 * than 0.
 * @param member_size The size of each element in bytes. Must be greater than 0.
 * @param cmp_fn Optional comparison function for elements, NULL if not needed.
 * @param destroy_fn Optional destructor function for elements, NULL if not
 * needed.
 * @param copy_fn Optional copy function for elements, NULL if not needed.
 *
 * @return DS_STATUS_OK on success
 * @return DS_STATUS_IS_NULL if stack is NULL
 * @return DS_STATUS_MEMBER_SIZE_0 if member_size is 0
 * @return DS_STATUS_SIZE_0 if initial_capacity is 0
 * @return DS_STATUS_OVERFLOW if initial_capacity exceeds `SIZE_MAX /
 * member_size`
 * @return DS_STATUS_ALLOC_FAIL if internal buffer allocation fails
 *
 * @note Use @ref ds_stack_deinit to free the internal buffer when done.
 *
 * @see ds_stack_deinit
 * @see ds_stack_destroy
 * @see ds_stack_init_default
 * @see ds_array::ds_array_init
 *
 * @ingroup stack_api
 */
static inline DS_STATUS ds_stack_init(ds_stack* stack, size_t initial_capacity,
                                      size_t member_size, ds_cmp_fn cmp_fn,
                                      ds_destroy_fn destroy_fn,
                                      ds_copy_fn copy_fn) {
    return ds_array_init(stack, initial_capacity, member_size, cmp_fn,
                         destroy_fn, copy_fn);
}

/**
 * @brief Initializes a stack with default settings.
 *
 * @param stack The stack to initialize. Must not be NULL.
 * @param member_size The size of each element in bytes. Must be greater than 0.
 *
 * @return DS_STATUS_OK on success
 * @return DS_STATUS_IS_NULL if stack is NULL
 * @return DS_STATUS_MEMBER_SIZE_0 if member_size is 0
 * @return DS_STATUS_ALLOC_FAIL if internal buffer allocation
 *
 * @note Use @ref ds_stack_deinit to free the internal buffer when done.
 *
 * @see ds_stack_deinit
 * @see ds_stack_destroy
 * @see ds_stack_init
 * @see ds_array::ds_array_init_default
 *
 * @ingroup stack_api
 */
static inline DS_STATUS ds_stack_init_default(ds_stack* stack,
                                              size_t member_size) {
    return ds_array_init_default(stack, member_size);
}

/**
 * @brief Deinitializes an existing stack.
 *
 * @param stack The stack to deinitialize. Must not be NULL.
 *
 * @note This function does not free the stack itself; it only deinitializes the
 * internal buffer.
 *
 * @see ds_stack_destroy
 * @see ds_array::ds_array_deinit
 *
 * @ingroup stack_api
 */
static inline void ds_stack_deinit(ds_stack* stack) { ds_array_deinit(stack); }

/**
 * @brief Destroys a stack and frees all associated resources.
 *
 * @param stack The stack to destroy. Must not be NULL.
 *
 * @note This function frees the stack itself and all associated resources.
 *
 * @see ds_stack_deinit
 * @see ds_array::ds_array_destroy
 *
 * @ingroup stack_api
 */
static inline void ds_stack_destroy(ds_stack* stack) {
    ds_array_destroy(stack);
}

/**
 * @brief Pushes an element onto the top of the stack.
 *
 * @param stack The stack to which the element will be pushed. Must not be NULL.
 * @param element The element to push onto the stack. Must not be NULL.
 *
 * @return DS_STATUS_OK on success
 * @return DS_STATUS_IS_NULL if array or element is NULL
 * @return DS_STATUS_ALLOC_FAIL if internal buffer allocation fails
 * @return DS_STATUS_OVERFLOW if resizing the array would exceed `SIZE_MAX /
 * member_size`
 *
 * @note The element is copied into the stack, so the caller retains ownership
 * of the original.
 *
 * @see ds_stack_pop
 * @see ds_array::ds_array_push
 *
 * @ingroup stack_api
 */
static inline DS_STATUS ds_stack_push(ds_stack* stack, const void* element) {
    return ds_array_push(stack, element);
}

/**
 * @brief Removes the top element from the stack and optionally retrieves it.
 *
 * @param stack The stack from which to remove the element. Must not be NULL.
 * @param out A pointer to a buffer where the removed element will be stored, or
 * NULL if the element is not needed.
 *
 * @note If the stack has a destroy function, it will be called on the removed
 * element after it is copied to `out`.
 * @note If the stack's length is less than capacity / 4, the array will be
 * resized to half its current capacity to save memory.
 *
 * @warning If the stack does not have a destroy function and out is set to
 * NULL, the removed element will be lost and may cause a memory leak if it was
 * dynamically allocated.
 *
 * @return DS_STATUS_OK on success
 * @return DS_STATUS_IS_NULL if stack is NULL
 * @return DS_STATUS_INDEX_OUT_OF_BOUNDS if index is out of bounds
 * @return DS_STATUS_OVERFLOW if resizing the array would exceed `SIZE_MAX /
 * member_size`
 * @return DS_STATUS_ALLOC_FAIL if internal buffer allocation fails
 *
 * @see ds_stack_push
 * @see ds_array::ds_array_pop
 *
 * @ingroup stack_api
 */
static inline DS_STATUS ds_stack_pop(ds_stack* stack, void* out) {
    return ds_array_pop(stack, out);
}

/**
 * @brief Returns the top element of the stack without removing it.
 *
 * @param stack The stack to peek at.
 * @return A pointer to the top element, or NULL if the stack is empty or NULL.
 *
 * @ingroup stack_api
 */
static inline void* ds_stack_peek(ds_stack* stack) {
    if (!stack || stack->length == 0) {
        return NULL;
    }
    return ds_array_get(stack, stack->length - 1);
}

/**
 * @brief Reverses the order of elements in the stack in place.
 *
 * @param stack The stack to reverse.
 *
 * @return DS_STATUS_OK on success
 * @return DS_STATUS_IS_NULL if stack is NULL
 * @return DS_STATUS_ALLOC_FAIL if internal buffer allocation fails during the
 * reversal process
 *
 * @ingroup stack_api
 */
static inline DS_STATUS ds_stack_reverse(ds_stack* stack) {
    return ds_array_reverse(stack);
}
#endif