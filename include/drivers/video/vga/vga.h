#pragma once

#include <stdint.h>

#define VGA_MEMORY 0xB8000

void vga_putchar(const char c);

void vga_set_char_color(uint8_t text_color, uint8_t background_color);
