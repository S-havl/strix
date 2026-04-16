#include <arch/x86_64/cpu/gdt.h>
// #include <arch/x86_64/idt.h>
// #include <arch/x86_64/pic.h>

// #include <drivers/timer/pit.h>
// #include <drivers/input/keyboard.h>

#include <kprintf.h>

void kernel_init() {
    clear_screen();

    kprintf("Starting kernel...\n");
    kprintf("Hello world from the kernel!\n");
    kprintf("Test1.\n");
    kprintf("Test2.\n");
    kprintf("Test3.\n");
    kprintf("Everything perfect.\n");

    init_gdt();
    kprintf("GDT initialized.\n");
    kprintf("TSS initialized.\n");
    kprintf("CS reloaded.\n");

    // idt_init();
    // kprintf("IDT initialized\n");

    // pic_init();
    // kprintf("PIC initialized\n");
    
    // pit_init();
    // kprintf("PIT initialized\n");

    // keyboard_init();
    // kprintf("KEYBOARD initialized\n");

    // asm volatile("sti");
    // kprintf("Interruptions enable.\n");
    
    while(1) {
        asm volatile("hlt");
    }
}
