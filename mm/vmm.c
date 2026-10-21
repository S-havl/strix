#include <kprintf.h>
#include <mm/pmm.h>
#include <mm/vmm.h>

#include <stddef.h>

#define PAGE_ALIGNED __attribute__((aligned(4096)))

typedef uint64_t pt_entry_t;

#define ENTRIES_PER_TABLE 512

// flags
#define PAGE_PRESENT (1ULL << 0)
#define PAGE_READ_WRITE (1ULL << 1)
#define PAGE_USER_SUPERVISOR (1ULL << 2)
#define PAGE_LEVEL_WRITE_THROUGH (1ULL << 3)
#define PAGE_LEVEL_CACHE_DISABLE (1ULL << 4)
#define PAGE_NO_EXECUTE_EXECUTE_DISABLE (1ULL << 63)

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

static page_table_t             pt;
static page_directory_t         pd;
static page_dir_pointer_table_t pdpt;
static pml4_table_t             pml4;

static void vmm_map(const uint64_t virtual_memory_address, const uint64_t physical_memory_address,
                    const uint64_t flags)
{
}

static void vmm_unmap(void) {}

static void vmm_context_switch(void) {}

static void vmm_flush_tlb(void) {}

void init_virtual_memory_manager(void) {}
