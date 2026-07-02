#pragma once

#ifdef DS_PANIC_ON_MALLOC
    #define DS_HANDLE_MALLOC_FAILURE(msg, retval) \
        do { \
            fprintf(stderr, "[LIB-DS-C FATAL] Out of memory: %s\n", msg); \
            exit(EXIT_FAILURE); \
        } while(0)
#else
    #define DS_HANDLE_MALLOC_FAILURE(msg, retval) return (retval)
#endif

typedef int (*ds_cmp_fn)(const void*, const void*);
typedef void (*ds_destroy_fn)(void*);