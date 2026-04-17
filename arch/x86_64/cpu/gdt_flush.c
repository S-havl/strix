#include <stdint.h>

void gdt_flush(void *gdtr_ptr) {
    asm volatile (
        "lgdt (%0)"
	:
	: "r"(gdtr_ptr)
	: "memory"
    );
}
