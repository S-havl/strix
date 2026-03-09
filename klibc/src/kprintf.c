#include <stdint.h>
#include <stddef.h>

#include "kprintf.h"

static volatile uint16_t* const VGA_MEMORY = (const volatile uint16_t*)0xB8000;

void kprintf(const uint8_t* str) {
    
}
