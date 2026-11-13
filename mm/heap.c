#include <mm/heap.h>
#include <mm/pmm.h>
#include <mm/vmm.h>

#include <stddef.h>
#include <stdint.h>

#pragma pack(push, 1)
typedef struct block_header {
    uint64_t block_size;
    uint8_t  is_free;
} block_header_t;

typedef struct block {
    block_header_t header;
    struct block*  next_block;
} block_t;
#pragma pack(pop)

void* kmalloc(void);

void kfree(void);

void heap_init(void)
{
    uint64_t bitmap_end_addr = pmm_get_bitmap_end_addr();
    uint64_t heap_start_addr = (bitmap_end_addr + 0xFFF) & ~0xFFF;

    block_t* initial_block     = (block_t*)heap_start_addr;
    uint64_t initial_heap_size = 16 * 1024 * 1024;

    initial_block->header.block_size = initial_heap_size - sizeof(block_t);
    initial_block->header.is_free    = 1;
    initial_block->next_block        = NULL;
}
