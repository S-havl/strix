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

#define ON_BIT 1
#define OFF_BIT 0

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

static pml4_table_t pml4;

static void vmm_flush_tlb(const uint64_t virtual_memory_address);

static void vmm_map(const uint64_t virtual_memory_address, const uint64_t physical_memory_address,
                    const uint64_t flags)
{
    uint64_t virtual_address_pml4_index = (virtual_memory_address >> 39) & 0x1FFULL;
    uint64_t virtual_address_pdpt_index = (virtual_memory_address >> 30) & 0x1FFULL;
    uint64_t virtual_address_pd_index   = (virtual_memory_address >> 21) & 0x1FFULL;
    uint64_t virtual_address_pt_index   = (virtual_memory_address >> 12) & 0x1FFULL;
    uint64_t virtual_address_offset     = (virtual_memory_address >> 0) & 0xFFFULL;

    uint64_t           pml4_entry = pml4.entries[virtual_address_pml4_index];
    volatile uint64_t* ptr_new_pdpt_physical_address;

    if (!(pml4_entry & PAGE_PRESENT)) {
        uint64_t new_pdpt_physical_address = pmm_alloc_page();
        ptr_new_pdpt_physical_address      = (volatile uint64_t*)new_pdpt_physical_address;

        for (size_t i = 0; i < 512; i++) {
            ptr_new_pdpt_physical_address[i] = 0x00ULL;
        }

        pml4.entries[virtual_address_pml4_index] =
            new_pdpt_physical_address | PAGE_PRESENT | PAGE_READ_WRITE;
    } else {
        ptr_new_pdpt_physical_address = (volatile uint64_t*)(pml4_entry & 0xFFFFFFFFFFFFF000ULL);
    }

    uint64_t           pdpt_entry = ptr_new_pdpt_physical_address[virtual_address_pdpt_index];
    volatile uint64_t* ptr_new_pd_physical_address;

    if (!(pdpt_entry & PAGE_PRESENT)) {
        uint64_t new_pd_physical_address = pmm_alloc_page();
        ptr_new_pd_physical_address      = (volatile uint64_t*)new_pd_physical_address;

        for (size_t i = 0; i < 512; i++) {
            ptr_new_pd_physical_address[i] = 0x00ULL;
        }

        ptr_new_pdpt_physical_address[virtual_address_pdpt_index] =
            new_pd_physical_address | PAGE_PRESENT | PAGE_READ_WRITE;
    } else {
        ptr_new_pd_physical_address = (volatile uint64_t*)(pdpt_entry & 0xFFFFFFFFFFFFF000ULL);
    }

    uint64_t           pd_entry = ptr_new_pd_physical_address[virtual_address_pd_index];
    volatile uint64_t* ptr_new_pt_physical_address;

    if (!(pd_entry & PAGE_PRESENT)) {
        uint64_t new_pt_physical_address = pmm_alloc_page();
        ptr_new_pt_physical_address      = (volatile uint64_t*)new_pt_physical_address;

        for (size_t i = 0; i < 512; i++) {
            ptr_new_pt_physical_address[i] = 0x00ULL;
        }

        ptr_new_pd_physical_address[virtual_address_pd_index] =
            new_pt_physical_address | PAGE_PRESENT | PAGE_READ_WRITE;
    } else {
        ptr_new_pt_physical_address = (volatile uint64_t*)(pd_entry & 0xFFFFFFFFFFFFF000ULL);
    }

    ptr_new_pt_physical_address[virtual_address_pt_index] = physical_memory_address | flags;
}

static void vmm_unmap(const uint64_t virtual_memory_address)
{
    uint64_t virtual_address_pml4_index = (virtual_memory_address >> 39) & 0x1FFULL;
    uint64_t virtual_address_pdpt_index = (virtual_memory_address >> 30) & 0x1FFULL;
    uint64_t virtual_address_pd_index   = (virtual_memory_address >> 21) & 0x1FFULL;
    uint64_t virtual_address_pt_index   = (virtual_memory_address >> 12) & 0x1FFULL;

    uint64_t pml4_entry = pml4.entries[virtual_address_pml4_index];
    if (!(pml4_entry & PAGE_PRESENT)) {
        return;
    }
    volatile uint64_t* pdpt = (volatile uint64_t*)(pml4_entry & 0xFFFFFFFFFFFFF000ULL);

    uint64_t pdpt_entry = pdpt[virtual_address_pdpt_index];
    if (!(pdpt_entry & PAGE_PRESENT)) {
        return;
    }
    volatile uint64_t* pd = (volatile uint64_t*)(pdpt_entry & 0xFFFFFFFFFFFFF000ULL);

    uint64_t pd_entry = pd[virtual_address_pd_index];
    if (!(pd_entry & PAGE_PRESENT)) {
        return;
    }
    volatile uint64_t* pt = (volatile uint64_t*)(pd_entry & 0xFFFFFFFFFFFFF000ULL);

    uint64_t pt_entry = pt[virtual_address_pt_index];
    if (!(pt_entry & PAGE_PRESENT)) {
        return;
    }

    uint64_t pt_physical_frame = pt[virtual_address_pt_index] & 0xFFFFFFFFFFFFF000ULL;
    pmm_free_page(pt_physical_frame);

    pt[virtual_address_pt_index] = 0x00ULL;

    vmm_flush_tlb(virtual_memory_address);
}

static void vmm_context_switch(void) {}

static void vmm_flush_tlb(const uint64_t virtual_memory_address)
{
    __asm__ __volatile__("invlpg (%0)" : : "r"(virtual_memory_address) : "memory");
}

void init_virtual_memory_manager(void) {}
