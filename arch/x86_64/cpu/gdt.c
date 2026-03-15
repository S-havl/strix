#include <stdint.h>
#include <gdt.h>

#pragma pack(push, 1)
struct GDTEntry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_high;
};

struct GDTR {
    uint16_t limit;
    uint64_t base;
};
#pragma pack(pop)

