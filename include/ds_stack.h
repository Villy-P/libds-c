#pragma once

#include "ds_array.h"

#ifdef DS_DEFINE_STACK
typedef ds_array ds_stack;

static inline ds_stack* ds_stack_create(size_t initial_capacity) {
    return ds_array_create(initial_capacity);
}

static inline bool ds_stack_init(ds_stack* stack, size_t initial_capacity) {
    return ds_array_init(stack, initial_capacity);
}

static inline void ds_stack_destroy(ds_stack* stack) {
    ds_array_destroy(stack);
}

static inline void ds_stack_destroy_with(ds_stack* stack, void (*free_fn)(void*)) {
    ds_array_destroy_with(stack, free_fn);
}

static inline bool ds_stack_push(ds_stack* stack, void* element) {
    return ds_array_push(stack, element);
}

static inline void* ds_stack_pop(ds_stack* stack) {
    return ds_array_pop(stack);
}

static inline void* ds_stack_peek(const ds_stack* stack) {
    if (!stack || stack->length == 0) {
        return NULL;
    }
    return ds_array_get(stack, stack->length - 1);
}
#endif