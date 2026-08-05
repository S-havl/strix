#include <stdint.h>
#include <stddef.h>
#include <drivers/video/vga/vga.h>
#include "kprintf.h"

static volatile uint16_t* const vga = (volatile uint16_t*)VGA_MEMORY;

void kprintf(const char* str)
{
    for (size_t i = 0; str[i] != '\0'; i++) {
        putchar(str[i]);
    }
}

void clear_screen()
{
    for (int i = 0; i < 80 * 25; i++) {
        vga[i] = 0x0720;
    }
}
