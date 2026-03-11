#include <stdint.h>
#include <stddef.h>
#include <kprintf.h>

void _start(void) {
    clear_screen();
    kprintf("Hello world from the kernel!\n");
    kprintf("Test1.\n");
    kprintf("Test2.\n");
    kprintf("Test3.\n");
    kprintf("Everything perfect.\n");
    while (1) {
        __asm__("hlt");
    }
}
