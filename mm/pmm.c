#include <mm/pmm.h>
#include <kprintf.h>

#include <stdint.h>

#define PAGE_SIZE 4096

uint8_t* bitmap = (uint8_t*)...;

static uint16_t* e820_count = (uint16_t*)E820_COUNT_ADDRESS;
static e820_entry_t* e820_map = (e820_entry_t*)E820_ENTRY_MAP_ADDRESS;

static int is_memory_usable(const uint32_t type)
{
    switch(type) {
        case ACPI_MEM_USABLE:
            return 1;

        case ACPI_MEM_ACPI_RECLAIM:
            return 0;

        case ACPI_MEM_RESERVED:
        case ACPI_MEM_NVS:
        case ACPI_MEM_UNUSABLE:
	case ACPI_MEM_DISABLED:
        case ACPI_MEM_UNACCEPTED:
        default:
            return 0;
    }
}

static void inspect_e820_map_entries(const e820_entry_t* e820_map, const uint16_t* e820_count)
{
    for (uint16_t i = 0; i < *e820_count; i++) { 
        uint64_t base_addr           = e820_map[i].base_addr;
        uint64_t length              = e820_map[i].length;
        uint32_t type                = e820_map[i].type;
        uint32_t extended_attributes = e820_map[i].extended_attributes;

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
    inspect_e820_map_entries(e820_map, e820_count);

    /*
    for (uint16_t i = 0; i < *e820_count; i++) {
        if (is_memory_usable(e820_map[i].type)) {
            kprintf(COLOR_WHITE, COLOR_BLACK, "\n");

            kprintf(COLOR_LIGHT_MAGENTA, COLOR_BLACK, "Base: ");
            kprintf_hex64(COLOR_LIGHT_BLUE, COLOR_BLACK, e820_map[i].base_addr);

            kprintf(COLOR_WHITE, COLOR_BLACK, "\n");

            kprintf(COLOR_LIGHT_MAGENTA, COLOR_BLACK, "Size: ");
            kprintf_hex64(COLOR_LIGHT_BLUE, COLOR_BLACK, e820_map[i].length);

            kprintf(COLOR_WHITE, COLOR_BLACK, "\n");
        }
    }
    */
}
