#include <arch/x86_64/cpu/gdt.h>
// #include <arch/x86_64/idt.h>
// #include <arch/x86_64/pic.h>

// #include <drivers/timer/pit.h>
// #include <drivers/input/keyboard.h>

#include <kprintf.h>

void kernel_init() {
    clear_screen();

    kprintf("[DATA] Starting kernel...\n");
    kprintf("[DATA] Hello world from the kernel!\n");
    kprintf("[DATA] Test1.\n");
    kprintf("[DATA] Test2.\n");
    kprintf("[DATA] Test3.\n");
    kprintf("[DATA] Everything perfect.\n");

    gdt_init();
    kprintf("[INFO] GDT initialized.\n");
    kprintf("[INFO] TSS initialized.\n");
    kprintf("[INFO] CS reloaded.\n");

    // idt_init();
    // kprintf("[INFO] IDT initialized\n");

    // pic_init();
    // kprintf("[INFO] PIC initialized\n");
    
    // pit_init();
    // kprintf("[INFO] PIT initialized\n");

    // keyboard_init();
    // kprintf("[INFO] KEYBOARD initialized\n");

    // asm volatile("sti");
    // kprintf("[INFO] Interruptions enable.\n");
    
    while(1) {
        asm volatile("hlt");
    }
}
