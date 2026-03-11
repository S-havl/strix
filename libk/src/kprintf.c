#include <stdint.h>
#include <stddef.h>
#include "kprintf.h"

#define VGA_MEMORY 0xB8000
#define VGA_COLOR 0x0F
#define VGA_WIDTH 80

static volatile uint16_t* const vga = (volatile uint16_t*)VGA_MEMORY;
static size_t cursor = 0;

void kprintf(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) {

        if (str[i] == '\n') {
            cursor += VGA_WIDTH - (cursor % VGA_WIDTH);
            continue;
        }

        if (cursor >= 80 * 25) cursor = 0;
        vga[cursor] = ((uint16_t)VGA_COLOR << 8) | (uint8_t)str[i];
        cursor++;
    }
}

void clear_screen() {
    for (int i = 0; i < 80 * 25; i++) {
        vga[i] = 0x0720;
  }
}
