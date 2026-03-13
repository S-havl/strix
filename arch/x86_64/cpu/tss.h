#ifndef TSS_H
#define TSS_H

#include <stdint.h>

struct GDTEntry64;

void set_gdt_tss(struct GDTEntry64 *entry, uint64_t base, uint32_t limit, uint8_t access, uint8_t flags);

#endif
