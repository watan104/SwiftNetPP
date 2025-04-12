#include "include/snpp/utils/Allocator.h"
#include <stdlib.h>

static void* default_malloc(size_t size) { return malloc(size); }
static void  default_free(void* ptr)    { free(ptr); }

static SNPP_Allocator active_allocator = {
    .malloc = default_malloc,
    .free = default_free
};

void snpp_set_allocator(const SNPP_Allocator* allocator) {
    if (allocator != NULL) {
        active_allocator.malloc = allocator->malloc ? allocator->malloc : default_malloc;
        active_allocator.free = allocator->free ? allocator->free : default_free;
    } else {
        active_allocator.malloc = default_malloc;
        active_allocator.free = default_free;
    }
}

void* snpp_malloc(size_t size) {
    return active_allocator.malloc(size);
}

void snpp_free(void* ptr) {
    if (ptr != NULL) {
        active_allocator.free(ptr);
    }
}