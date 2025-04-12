#pragma once
#include "../core.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    void* (*malloc)(size_t);
    void (*free)(void*);
} SNPP_Allocator;

SNPP_API void snpp_set_allocator(const SNPP_Allocator* allocator);

#ifdef __cplusplus
}
#endif