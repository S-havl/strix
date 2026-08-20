#include <stdint.h>

void gdt_flush(void *gdtr_ptr)
{
    __asm__ __volatile__ (
        "lgdt (%0)"
	:
	: "r"(gdtr_ptr)
	: "memory"
    );
}
