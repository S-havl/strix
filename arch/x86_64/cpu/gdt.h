#ifndef GDT_H
#define GDT_H

#include <stdint.h>

struct GDTEntry;

struct GDTR;

void set_gdt_entry(struct GDTEntry *entry, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags);


#endif
