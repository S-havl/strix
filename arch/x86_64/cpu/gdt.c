#include <stdint.h>
#include <gdt.h>

#define GDT_SIZE 5

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

struct GDTEntry gdt[GDTEntry];

void set_gdt_entry(struct GDTEntry *entry, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags) {
    entry->base_low    = base & 0xFFFF;
    entry->base_middle = (base >> 16) & 0xFF;
    entry->base_high   = (base >> 24) & 0xFF;
    entry->limit_low   = limit & 0xFFFF;
    entry->granularity = ((limit >> 16) & 0x0F) | (flags & 0xF0);
    entry->access      = access;
}
