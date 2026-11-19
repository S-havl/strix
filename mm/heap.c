#include <mm/heap.h>
#include <mm/pmm.h>
#include <mm/vmm.h>

#include <stddef.h>
#include <stdint.h>

typedef struct block_header {
    uint64_t block_size;
    uint8_t  is_free;
} block_header_t;

typedef struct __attribute__((aligned(16))) block {
    block_header_t header;
    struct block*  next_block;
} block_t;

static block_t* head_block = NULL;

void* kmalloc(size_t size)
{
    if (size == 0) {
        return NULL;
    }

    size = (size + 15) & ~((size_t)15);

    block_t* current_block = head_block;

    while (current_block != NULL) {
        if (current_block->header.is_free == 1 && current_block->header.block_size >= size) {
            if (current_block->header.block_size > size + sizeof(block_t)) {
                block_t* next_free_block =
                    (block_t*)((uint8_t*)current_block + sizeof(block_t) + size);

                next_free_block->header.block_size =
                    current_block->header.block_size - size - sizeof(block_t);
                next_free_block->header.is_free = 1;
                next_free_block->next_block     = current_block->next_block;

                current_block->header.block_size = size;
                current_block->next_block        = next_free_block;
            }

            current_block->header.is_free = 0;
            return (void*)((uint8_t*)current_block + sizeof(block_t));
        }

        current_block = current_block->next_block;
    }

    return NULL;
}

void kfree(void* ptr)
{
    if (ptr == NULL) {
        return;
    }

    block_t* header_ptr_block        = (block_t*)((uint8_t*)ptr - sizeof(block_t));
    header_ptr_block->header.is_free = 1;

    while (header_ptr_block->next_block != NULL &&
           header_ptr_block->next_block->header.is_free == 1) {
        block_t* next_ptr_block = header_ptr_block->next_block;

        header_ptr_block->header.block_size += next_ptr_block->header.block_size + sizeof(block_t);

        header_ptr_block->next_block = next_ptr_block->next_block;
    }
}

void heap_init(void)
{
    uint64_t bitmap_end_addr = pmm_get_bitmap_end_addr();
    uint64_t heap_start_addr = (bitmap_end_addr + 0xFFF) & ~0xFFF;

    head_block                 = (block_t*)heap_start_addr;
    uint64_t initial_heap_size = 16 * 1024 * 1024;

    head_block->header.block_size = initial_heap_size - sizeof(block_t);
    head_block->header.is_free    = 1;
    head_block->next_block        = NULL;
}
