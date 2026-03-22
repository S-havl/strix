#include <stdint.h>
#include <tss.h>

struct GDTEntry64 {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_high;
    uint32_t base_upper;
    uint32_t reserved;
} __attribute__((packed));

struct TSS {
    uint32_t reserved0;
    uint64_t rsp0;
    uint64_t rsp1;
    uint64_t rsp2;
    uint64_t reserved1;
    uint64_t ist1;
    uint64_t ist2;
    uint64_t ist3;
    uint64_t ist4;
    uint64_t ist5;
    uint64_t ist6;
    uint64_t ist7;
    uint64_t reserved2;
    uint16_t reserved3;
    uint16_t io_map_base;
} __attribute__((packed));

struct GDTEntry64 tss_entry;

void set_gdt_tss(struct GDTEntry64 *entry, uint64_t base, uint32_t limit, uint8_t access, uint8_t flags) {
    entry->base_low     = base & 0xFFFF;
    entry->base_middle  = (base >> 16) & 0xFF;
    entry->base_high    = (base >> 24) & 0xFF;
    entry->base_upper   = (base >> 32) & 0xFFFFFFFF;
    entry->limit_low    = limit & 0xFFFF;
    entry->granularity  = ((limit >> 16) & 0x0F) | (flags & 0xF0);
    entry->access       = access;
    entry->reserved     = 0;
}

uint64_t tss_base = (uint64_t)&tss;
set_gdt_tss(&tss_entry, tss_base, sizeof(tss)-1, 0x89, 0x00);
