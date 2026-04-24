#include <stdint.h>
#include <arch/x86_64/interrupts/idt.h>

#define IDT_SIZE 256
#define KERNEL_CS 0x08

#pragma pack(push, 1)
struct IDTEntry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t  ist;
    uint8_t  type_attributes;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t zero;
};

struct IDTR {
    uint16_t limit;
    uint64_t base;
};
#pragma pack(pop)

struct IDTEntry idt[IDT_SIZE];
struct IDTR idtr;

static void set_idt_entry(struct IDTEntry *entry, uint64_t offset, uint16_t selector, uint8_t ist, uint8_t type_attributes) {
    *entry = (struct IDTEntry){0};

    entry->offset_low      = offset & 0xFFFF;
    entry->selector        = selector;
    entry->ist             = ist & 0x7;
    entry->type_attributes = type_attributes;
    entry->offset_mid      = (offset >> 16) & 0xFFFF;
    entry->offset_high     = (offset >> 32) & 0xFFFFFFFF;
    entry->zero            = 0;
}

void idt_init(void) {
    idtr.limit = sizeof(idt) - 1;
    idtr.base  = (uint64_t)&idt;

    for (uint32_t i = 0; i < IDT_SIZE; i++) {
        set_itd_entry(&idt[i], /* handler offset */, KERNEL_CS, 0, 0x8E);
    }
}

