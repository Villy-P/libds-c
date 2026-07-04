/**
 * @file libds_c.h
 * @brief Central header file for the libds-c library, including all data structure headers.
 */

/**
 * @def DS_C_IMPLEMENTATION
 * @brief Define this in exactly one .c file before including the any header
 * @par Usage
 * Only one translation unit should define this. Defining it in multiple
 * .c files will cause linker errors due to duplicate symbols.
 *
 * @code
 * // my_file.c
 * #define DS_C_IMPLEMENTATION
 * #include "libds_c.h"
 * @endcode
 *
 * @warning Defining this in a header file will cause duplicate symbol
 *          errors at link time.
 */
#ifdef DOXYGEN
    #define DS_C_IMPLEMENTATION
#endif

#pragma once

#include "ds_array.h"
#include "ds_string.h"
#include "ds_common.h"
#include "ds_stack.h"