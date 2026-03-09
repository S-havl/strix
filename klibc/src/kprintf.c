#include <stdint.h>
#include "kprintf.h"

static volatile uint16_t* const VGA_MEMORY = (const volatile uint16_t*)B8000;

void kprintf(const uint8_t* str) {
    
}
