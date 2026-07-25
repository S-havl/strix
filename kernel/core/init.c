#include <arch/x86_64/cpu/gdt.h>
#include <arch/x86_64/interrupts/idt.h>
#include <kernel/core/interrupt.h>
// #include <arch/x86_64/pic.h>

// #include <drivers/timer/pit.h>
// #include <drivers/input/keyboard.h>

#include <kprintf.h>

void kernel_init()
{
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

    idt_init();
    kprintf("[INFO] IDT initialized.\n");

    interrupt_handlers_init();
    kprintf("[INFO] IDT HANDLERS initialized.\n");

    asm volatile ("int $0");
    asm volatile ("int $1");
    asm volatile ("int $2");
    asm volatile ("int $3");
    asm volatile ("int $4");
    asm volatile ("int $5");
    asm volatile ("int $6");
    asm volatile ("int $7");
    asm volatile ("int $8");
    asm volatile ("int $9");
    asm volatile ("int $10");
    asm volatile ("int $11");
    asm volatile ("int $12");
    asm volatile ("int $13");
    asm volatile ("int $14");
    asm volatile ("int $15");
    asm volatile ("int $16");
    asm volatile ("int $17");
    asm volatile ("int $18");

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
