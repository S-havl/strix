#include <stdint.h>
#include <stddef.h>
#include <drivers/video/vga/vga.h>
#include "kprintf.h"

static volatile uint16_t* const vga = (volatile uint16_t*)VGA_MEMORY;

void kprintf(uint8_t text_color, uint8_t background_color, const char* str)
{
    vga_set_char_color(text_color, background_color);

    for (size_t i = 0; str[i] != '\0'; i++) {
        vga_putchar(str[i]);
    }
}

void clear_screen()
{
    for (int i = 0; i < 80 * 25; i++) {
        vga[i] = 0x0720;
    }
}
