/**
 * @file ds_common.h
 * @brief Contains project-wide declarations and definitions for the libds-c library.
 */

#pragma once

#ifdef DS_PANIC_ON_MALLOC
    #define DS_HANDLE_MALLOC_FAILURE(msg, retval) \
        do { \
            fprintf(stderr, "[LIB-DS-C FATAL] %s (%s:%d): %s\n", \
                __func__, __FILE__, __LINE__, msg); \
            exit(EXIT_FAILURE); \
        } while(0)
#else
    #define DS_HANDLE_MALLOC_FAILURE(msg, retval) return (retval)
#endif

typedef int (*ds_cmp_fn)(const void*, const void*);
typedef void (*ds_destroy_fn)(void*);

typedef enum {
    DS_STATUS_OK,
    DS_STATUS_IS_NULL,
    DS_STATUS_SIZE_0,
    DS_STATUS_MEMBER_SIZE_0,
    DS_STATUS_MEMBER_SIZE_MISMATCH,
    DS_STATUS_OVERFLOW,
    DS_STATUS_ALLOC_FAIL,
    DS_STATUS_OUT_OF_BOUNDS,
    DS_STATUS_ERROR
} DS_STATUS;

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