#include <kprintf.h>
#include <mm/pmm.h>

#include <stddef.h>

#define BIT_FREE 0
#define BIT_USED 1

#define BYTE_FREE 0x00
#define BYTE_USED 0xFF

extern uint8_t  _kernel_end[];
static uint8_t* pmm_bitmap = NULL;

static size_t total_ram_pages   = 0;
static size_t total_bitmap_size = 0;

static const uint16_t*     e820_count = (uint16_t*)E820_COUNT_ADDRESS;
static const e820_entry_t* e820_map   = (e820_entry_t*)E820_ENTRY_MAP_ADDRESS;

static inline bool is_memory_usable(acpi_memory_type_t type) { return type == ACPI_MEM_USABLE; }

static void pmm_lock_page(size_t page_number)
{
    if (page_number >= total_ram_pages) {
        return;
    }

    size_t byte_index = page_number / 8;
    size_t bit_index  = page_number % 8;

    if (byte_index >= total_bitmap_size) {
        return;
    }

    pmm_bitmap[byte_index] |= (1 << bit_index);
}

static void pmm_lock_region(uint64_t start_addr, uint64_t length)
{
    size_t start_page = start_addr / PAGE_SIZE;
    size_t end_page   = (start_addr + length) / PAGE_SIZE;

    for (size_t page = start_page; page < end_page; page++) {
        pmm_lock_page(page);
    }
}

static uint64_t pmm_alloc_page(void)
{
    for (size_t bitmap_byte_index = 0; bitmap_byte_index < total_bitmap_size; bitmap_byte_index++) {
        if (pmm_bitmap[bitmap_byte_index] == BYTE_USED) {
            continue;
        }

        for (size_t bit_offset = 0; bit_offset < 8; bit_offset++) {
            size_t global_page_index = (bitmap_byte_index * 8) + bit_offset;

            if (global_page_index >= total_ram_pages) {
                return (uint64_t)-1;
            }

            if (((pmm_bitmap[bitmap_byte_index] >> bit_offset) & 1) == BIT_FREE) {
                pmm_bitmap[bitmap_byte_index] |= (BIT_USED << bit_offset);

                uint64_t free_page_physical_address = (uint64_t)global_page_index * PAGE_SIZE;
                return free_page_physical_address;
            }
        }
    }

    return (uint64_t)-1;
}

static void pmm_free_page(void) {}

static void inspect_e820_map_entries(const e820_entry_t* map, const uint16_t* count)
{
    for (uint16_t i = 0; i < *count; i++) {
        uint64_t base_addr = map[i].base_addr;
        uint64_t length    = map[i].length;
        uint32_t type      = map[i].type;

        kprintf(COLOR_LIGHT_MAGENTA, COLOR_BLACK, " B: ");
        kprintf_hex64(COLOR_LIGHT_BLUE, COLOR_BLACK, base_addr);

        kprintf(COLOR_LIGHT_MAGENTA, COLOR_BLACK, " L: ");
        kprintf_hex64(COLOR_LIGHT_BLUE, COLOR_BLACK, length);

        kprintf(COLOR_LIGHT_MAGENTA, COLOR_BLACK, " T: ");
        kprintf_hex64(COLOR_LIGHT_BLUE, COLOR_BLACK, type);

        kprintf(COLOR_WHITE, COLOR_BLACK, "\n");
    }
}

void init_physical_memory_map(void)
{
    pmm_bitmap = (uint8_t*)_kernel_end;

    inspect_e820_map_entries(e820_map, e820_count);

    uint64_t total_ram_bytes = 0;

    for (size_t i = 0; i < *e820_count; i++) {
        if (is_memory_usable((acpi_memory_type_t)e820_map[i].type)) {
            total_ram_bytes += e820_map[i].length;
            total_ram_pages += (e820_map[i].length / PAGE_SIZE);
        }
    }

    total_bitmap_size = (total_ram_pages + 7) / 8;

    for (size_t i = 0; i < total_bitmap_size; i++) {
        pmm_bitmap[i] = BYTE_FREE;
    }

    uint64_t kernel_start_addr   = KERNEL_START_ADDR;
    uint64_t bitmap_end_addr     = (uint64_t)pmm_bitmap + total_bitmap_size;
    uint64_t dynamic_kernel_size = bitmap_end_addr - kernel_start_addr;

    pmm_lock_region(kernel_start_addr, dynamic_kernel_size);

    for (size_t i = 0; i < *e820_count; i++) {
        if (e820_map[i].type != ACPI_MEM_USABLE) {
            pmm_lock_region(e820_map[i].base_addr, e820_map[i].length);
        }
    }

    kprintf(COLOR_LIGHT_MAGENTA, COLOR_BLACK, "TRB: ");
    kprintf_hex64(COLOR_LIGHT_BLUE, COLOR_BLACK, total_ram_bytes);

    kprintf(COLOR_LIGHT_MAGENTA, COLOR_BLACK, " TRP: ");
    kprintf_hex64(COLOR_LIGHT_BLUE, COLOR_BLACK, total_ram_pages);

    kprintf(COLOR_LIGHT_MAGENTA, COLOR_BLACK, " TBS: ");
    kprintf_hex64(COLOR_LIGHT_BLUE, COLOR_BLACK, total_bitmap_size);
}
