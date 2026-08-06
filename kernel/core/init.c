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

    // Remove all 'asm volatile' statements for production
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
    asm volatile ("int $19");
    asm volatile ("int $20");
    asm volatile ("int $21");
    asm volatile ("int $22");
    asm volatile ("int $23");
    asm volatile ("int $24");
    asm volatile ("int $25");
    asm volatile ("int $26");
    asm volatile ("int $27");
    asm volatile ("int $28");
    asm volatile ("int $29");
    asm volatile ("int $30");
    asm volatile ("int $31");

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
