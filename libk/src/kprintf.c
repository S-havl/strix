#include <stdint.h>
#include <stddef.h>
#include "kprintf.h"

#define VGA_MEMORY 0xB8000
#define VGA_COLOR 0x0F

static volatile uint16_t* const vga = (volatile uint16_t*)VGA_MEMORY;

void kprintf(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        vga[i] = ((uint16_t)VGA_COLOR << 8) | (uint8_t)str[i];
    }
}

void clear_screen() {
    for (int i = 0; i < 80 * 25; i++) {
        vga[i] = 0x0720;
  }
}
