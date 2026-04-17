#include <stdint.h>
#include <arch/x86_64/interrupts/idt.h>

#pragma pack(push, 1)
struct IDTEntry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t  ist;
    uint8_t  type_attr;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t zero;
};



