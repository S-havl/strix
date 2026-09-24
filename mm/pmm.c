#include <kprintf.h>
#include <mm/pmm.h>

#include <stddef.h>

extern uint8_t  _kernel_end[];
static uint8_t* pmm_bitmap = NULL;

static const uint16_t*     e820_count = (uint16_t*)E820_COUNT_ADDRESS;
static const e820_entry_t* e820_map   = (e820_entry_t*)E820_ENTRY_MAP_ADDRESS;

static inline bool is_memory_usable(acpi_memory_type_t type) { return type == ACPI_MEM_USABLE; }

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

    uint64_t total_ram_bytes   = 0;
    uint64_t total_ram_pages   = 0;
    uint64_t total_bitmap_size = 0;

    for (size_t i = 0; i < *e820_count; i++) {
        if (is_memory_usable((acpi_memory_type_t)e820_map[i].type)) {
            total_ram_bytes += e820_map[i].length;
            total_ram_pages += (e820_map[i].length / PAGE_SIZE);
        }
    }

    total_bitmap_size = (total_ram_pages + 7) / 8;

    kprintf(COLOR_LIGHT_MAGENTA, COLOR_BLACK, "TRB: ");
    kprintf_hex64(COLOR_LIGHT_BLUE, COLOR_BLACK, total_ram_bytes);

    kprintf(COLOR_LIGHT_MAGENTA, COLOR_BLACK, " TRP: ");
    kprintf_hex64(COLOR_LIGHT_BLUE, COLOR_BLACK, total_ram_pages);

    kprintf(COLOR_LIGHT_MAGENTA, COLOR_BLACK, " TBS: ");
    kprintf_hex64(COLOR_LIGHT_BLUE, COLOR_BLACK, total_bitmap_size);
}
