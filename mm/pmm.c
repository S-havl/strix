#include <mm/pmm.h>

#include <stdint.h>

static int is_memory_usable(uint32_t type)
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

void init_physical_memory_map(void)
{
    uint16_t* e820_count = (uint16_t*)E820_COUNT_ADDRESS;
    e820_entry_t* e820_map = (e820_entry_t*)E820_ENTRY_MAP_ADDRESS;

    for (uint16_t i = 0; i < *e820_count; i++) {
        if (is_memory_usable(e820_map[i].type)) {
        }
    }
}
