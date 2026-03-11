#include <stdint.h>
#include <stddef.h>
#include <kprintf.h>

void _start(void) {
    clear_screen();
    kprintf("Hello world from the kernel!");
    while (1) {
        __asm__("hlt");
    }
}
