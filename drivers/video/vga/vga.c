#include <stdint.h>
#include <stddef.h>
#include <drivers/video/vga/vga.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_COLOR 0x0A

static volatile uint16_t* const vga = (volatile uint16_t*)VGA_MEMORY;
static size_t cursor = 0;

void putchar(const char c)
{ 
    if (c == '\n') {
        cursor += VGA_WIDTH - (cursor % VGA_WIDTH);

        if (cursor >= VGA_WIDTH * VGA_HEIGHT) cursor = 0;

        return;
    }

    if (cursor >= VGA_WIDTH * VGA_HEIGHT) cursor = 0;

    vga[cursor] = ((uint16_t)VGA_COLOR << 8) | (uint8_t)c;
    cursor++;
}
