#include <kprintf.h>
#include <mm/pmm.h>
#include <mm/vmm.h>

#include <stddef.h>

#define PAGE_ALIGNED __attribute__((aligned(4096)))

typedef uint64_t pt_entry_t;

#define ENTRIES_PER_TABLE 512

typedef struct PAGE_ALIGNED {
    pt_entry_t entries[ENTRIES_PER_TABLE];
} page_table_t;

typedef struct PAGE_ALIGNED {
    pt_entry_t entries[ENTRIES_PER_TABLE];
} page_directory_t;

typedef struct PAGE_ALIGNED {
    pt_entry_t entries[ENTRIES_PER_TABLE];
} page_dir_pointer_table_t;

typedef struct PAGE_ALIGNED {
    pt_entry_t entries[ENTRIES_PER_TABLE];
} pml4_table_t;

_Static_assert(sizeof(page_table_t) == PAGE_SIZE, "PT must be 4096 bytes in size");
_Static_assert(sizeof(page_directory_t) == PAGE_SIZE, "PD must be 4096 bytes in size");
_Static_assert(sizeof(page_dir_pointer_table_t) == PAGE_SIZE, "PDPT must be 4096 bytes in size");
_Static_assert(sizeof(pml4_table_t) == PAGE_SIZE, "PML4 must be 4096 bytes in size");

void init_virtual_memory_manager(void) {}
