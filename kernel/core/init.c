#include <arch/x86_64/cpu/gdt.h>
#include <arch/x86_64/interrupts/idt.h>
#include <kernel/core/interrupt.h>
// #include <arch/x86_64/interrupts/pic.h>

// #include <drivers/timer/pit.h>
// #include <drivers/input/keyboard.h>

#include <kprintf.h>

void kernel_init()
{
    clear_screen();

    kprintf(COLOR_WHITE, COLOR_BLACK, "[INFO] Starting kernel...\n");
    kprintf(COLOR_WHITE, COLOR_BLACK, "[INFO] Hello world from the kernel!\n");
    kprintf(COLOR_WHITE, COLOR_BLACK, "[INFO] Test1.\n");
    kprintf(COLOR_WHITE, COLOR_BLACK, "[INFO] Test2.\n");
    kprintf(COLOR_WHITE, COLOR_BLACK, "[INFO] Test3.\n");
    kprintf(COLOR_WHITE, COLOR_BLACK, "[INFO] Everything perfect.\n");

    gdt_init();
    kprintf(COLOR_LIGHT_GREEN, COLOR_BLACK, "[ OK ] GDT initialized.\n");
    kprintf(COLOR_LIGHT_GREEN, COLOR_BLACK, "[ OK ] TSS initialized.\n");
    kprintf(COLOR_LIGHT_GREEN, COLOR_BLACK, "[ OK ] CS reloaded.\n");

    idt_init();
    kprintf(COLOR_LIGHT_GREEN, COLOR_BLACK, "[ OK ] IDT initialized.\n");

    interrupt_handlers_init();
    kprintf(COLOR_LIGHT_GREEN, COLOR_BLACK, "[ OK ] IDT HANDLERS initialized.\n");

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
