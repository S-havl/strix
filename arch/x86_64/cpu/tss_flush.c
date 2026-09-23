#include <arch/x86_64/cpu/tss.h>
#include <stdint.h>

void tss_flush(uint16_t selector) { __asm__ __volatile__("ltr %%ax" : : "a"(selector) : "memory"); }
