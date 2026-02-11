#include <stdint.h>

#define VIDEO_MEMORY 0xB8000
#define WHITE_ON_BLACK 0x0F

void kprint(const char *str) {
    volatile char* vga = (volatile char*)VIDEO_MEMORY;
    for (int i = 0; str[i] != '\0'; i++) {
        vga[i*2] = str[i];
        vga[i*2+1] = WHITE_ON_BLACK;
    }
}

void _start(void) {
    kprint("Hello world from the kernel!");
    while (1) {
        __asm__("hlt");
    }
}
