#include <arch/x86_64/cpu/gdt.h>
#include <arch/x86_64/interrupts/idt.h>
#include <arch/x86_64/interrupts/pic.h>
#include <kernel/core/interrupt.h>
#include <mm/heap.h>
#include <mm/pmm.h>
#include <mm/vmm.h>

// #include <drivers/timer/pit.h>
// #include <drivers/input/keyboard.h>

#include <kprintf.h>

void kernel_init(void)
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

    init_physical_memory_manager();
    kprintf(COLOR_LIGHT_GREEN, COLOR_BLACK, "[ OK ] Physical Memory Manager initialized.\n");

    init_virtual_memory_manager();
    kprintf(COLOR_LIGHT_GREEN, COLOR_BLACK, "[ OK ] Virtual Memory Manager initialized\n");

    pic_init();
    kprintf(COLOR_LIGHT_GREEN, COLOR_BLACK, "[ OK ] PIC initialized and remapped.\n");

    heap_init();
    kprintf(COLOR_LIGHT_GREEN, COLOR_BLACK, "[ OK ] HEAP initialized.\n");

    uint64_t* data = (uint64_t*)kmalloc(64);
    *data          = 0x1234567891011211ULL;
    kprintf_hex64(COLOR_WHITE, COLOR_BLACK, (uint64_t)(uintptr_t)data);
    kprintf_hex64(COLOR_LIGHT_GREEN, COLOR_BLACK, *data);
    kfree(data);

    uint64_t* reused = (uint64_t*)kmalloc(64);
    *reused          = 0xAABBCCDDEEFF0011ULL;
    kprintf_hex64(COLOR_WHITE, COLOR_BLACK, (uint64_t)(uintptr_t)reused);
    kprintf_hex64(COLOR_LIGHT_GREEN, COLOR_BLACK, *reused);
    kfree(reused);

    // pit_init();
    // kprintf("[INFO] PIT initialized\n");

    // keyboard_init();
    // kprintf("[INFO] KEYBOARD initialized\n");

    // asm volatile("sti");
    // kprintf("[INFO] Interruptions enable.\n");

    while (1) {
        __asm__ __volatile__("sti; hlt" : : : "memory");
    }
}
