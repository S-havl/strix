#include <stdint.h>
#include <arch/x86_64/interrupts/idt.h>

#define IDT_SIZE 256
#define KERNEL_CS 0x08

#pragma pack(push, 1)
typedef struct IDTEntry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t  ist;
    uint8_t  type_attributes;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t zero;
} IDTEntry_t;

typedef struct IDTR {
    uint16_t limit;
    uint64_t base;
} IDTR_t;
#pragma pack(pop)

IDTEntry_t idt[IDT_SIZE];
IDTR_t idtr;

static void set_idt_entry(IDTEntry_t *entry, uint64_t offset, uint16_t selector, uint8_t ist, uint8_t type_attributes)
{
    *entry = (IDTEntry_t){0};

    entry->offset_low      = offset & 0xFFFF;
    entry->selector        = selector;
    entry->ist             = ist & 0x7;
    entry->type_attributes = type_attributes;
    entry->offset_mid      = (offset >> 16) & 0xFFFF;
    entry->offset_high     = (offset >> 32) & 0xFFFFFFFF;
    entry->zero            = 0;
}

extern void isr0(void);
extern void isr1(void);
extern void isr2(void);

void idt_init(void)
{
    idtr.limit = sizeof(idt) - 1;
    idtr.base  = (uint64_t)&idt;

    set_idt_entry(&idt[0], (uint64_t)(uintptr_t)isr0, KERNEL_CS, 0, 0x8E);
    set_idt_entry(&idt[1], (uint64_t)(uintptr_t)isr1, KERNEL_CS, 0, 0x8E);
    set_idt_entry(&idt[2], (uint64_t)(uintptr_t)isr2, KERNEL_CS, 0, 0x8E);
    
    __asm__ volatile ("lidt %0" : : "m"(idtr));
}

