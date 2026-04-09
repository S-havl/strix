#pragma once
#include <stdint.h>

struct GDTR {
   uint16_t limit; 
   uint64_t base;
};

void init_gdt(void);
void gdt_flush(struct GDTR *gdtr_ptr);
