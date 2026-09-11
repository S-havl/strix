#pragma once

#include <stdint.h>

#define E820_COUNT_ADDRESS 0x6002
#define E820_ENTRY_MAP_ADDRESS 0x6004
#define MAX_E820_ENTRYS 256

#pragma pack(push, 1)
typedef struct e820_entry {
    uint64_t base_addr;
    uint64_t length;
    uint32_t type;
    uint32_t extended_attributes;
} e820_entry_t;
#pragma pack(pop)

typedef enum {
    ACPI_MEM_USABLE             = 1,
    ACPI_MEM_RESERVED           = 2,
    ACPI_MEM_ACPI_RECLAIM       = 3,
    ACPI_MEM_NVS                = 4,
    ACPI_MEM_UNUSABLE           = 5,
    ACPI_MEM_DISABLED           = 6,
    ACPI_MEM_PERSISTENT         = 7,
    ACPI_MEM_UNACCEPTED         = 8,

    ACPI_MEM_OEM_DEFINED        = 12,

    ACPI_MEM_OEM_DEFINED_HIGH   = 0xF0000000 // to 0xFFFFFFFF
} acpi_memory_type_t;

void init_physical_memory_map(void);
