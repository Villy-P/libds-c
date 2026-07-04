/**
 * @file ds_common.h
 * @brief Contains project-wide declarations and definitions for the libds-c library.
 */

#pragma once

/**
 * @brief Function pointer type for comparing two elements.
 */
typedef int (*ds_cmp_fn)(const void*, const void*);
/**
 * @brief Function pointer type for destroying an element, used for cleanup in data structures.
 */
typedef void (*ds_destroy_fn)(void*);

/**
 * @brief Enumeration of status codes used throughout the library.
 */
typedef enum {
    DS_STATUS_OK,                    /**< Operation completed successfully. */
    DS_STATUS_IS_NULL,               /**< A required pointer argument was NULL. */
    DS_STATUS_SIZE_0,                /**< Size argument was 0. */
    DS_STATUS_MEMBER_SIZE_0,         /**< Member size was 0. */
    DS_STATUS_MEMBER_SIZE_MISMATCH,  /**< Member sizes of two arrays do not match. */
    DS_STATUS_OVERFLOW,              /**< Operation would overflow size_t. */
    DS_STATUS_ALLOC_FAIL,            /**< Memory allocation failed. */
    DS_STATUS_OUT_OF_BOUNDS,         /**< Index exceeds array length. */
    DS_STATUS_ERROR                  /**< Generic unrecoverable error. */
} DS_STATUS;

/**
 * @def DS_THROWS_ERROR
 * @brief Controls error handling behaviour across the library.
 *
 * If defined, all errors print to stderr and call exit().
 * If not defined, all errors return a @ref DS_STATUS code or NULL.
 *
 * @code
 * #define DS_THROWS_ERROR
 * #include "libds_c.h"
 * @endcode
 *
 * @warning When defined, callers cannot recover from errors.
 */
#ifdef DOXYGEN
    #define DS_THROWS_ERROR
#endif

/**
 * @def DS_HANDLE_FAILURE
 * @brief Internal error handling macro.
 *
 * Behaviour depends on whether @ref DS_THROWS_ERROR is defined:
 * - Defined: prints to stderr and calls exit().
 * - Undefined: returns retval to the caller.
 *
 * @internal
 *
 * @param msg    Human readable error message.
 * @param retval Value to return if not throwing.
 *
 * @see DS_THROWS_ERROR
 */

#ifdef DS_THROWS_ERROR
    #define DS_HANDLE_FAILURE(msg, retval) \
        do { \
            fprintf(stderr, "[LIB-DS-C FATAL] %s (%s:%d): %s\n", \
                __func__, __FILE__, __LINE__, msg); \
            exit(EXIT_FAILURE); \
        } while(0)
#else
    #define DS_HANDLE_FAILURE(msg, retval) return (retval)
#endif