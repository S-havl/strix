#include <stdint.h>
#include <stddef.h>
#include <kprintf.h>

void _start(void) {
    kprintf("Hello world from the kernel!");
    while (1) {
        __asm__("hlt");
    }
}
