#pragma once

#include <stddef.h>
#include <stdint.h>

#define HEAP_INITIAL_SIZE (16ULL * 1024ULL * 1024ULL)

uint64_t heap_get_start_addr(void);
void     heap_init(void);
void*    kmalloc(size_t size);
void     kfree(void* ptr);
