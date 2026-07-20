#include <kernel/core/interrupt.h>
#include <arch/x86_64/interrupts/interrupt_frame.h>

#include <stdint.h>
#include <stddef.h>
#include <kprintf.h>

#define MAX_GATES 256

interrupt_handler_t interrupt_handlers[MAX_GATES] = {NULL};

void register_interrupt_handler(uint8_t n, interrupt_handler_t handler)
{
    interrupt_handlers[n] = handler;
}

// Handlers for idt gates
void divide_error_handler(interrupt_frame_t *frame)
{
    kprintf("#DE\n");
}

void debug_exception_handler(interrupt_frame_t *frame)
{
    kprintf("#DB\n");
}

void nmi_interrupt_handler(interrupt_frame_t *frame)
{
    kprintf("NMI\n");
}

void breakpoint_handler(interrupt_frame_t *frame)
{
    kprintf("#BP\n");
}

void overflow_handler(interrupt_frame_t *frame)
{
    kprintf("#OF\n");
}

void interrupt_handlers_init(void)
{
    register_interrupt_handler(0, divide_error_handler);
    register_interrupt_handler(1, debug_exception_handler);
    register_interrupt_handler(2, nmi_interrupt_handler);
    register_interrupt_handler(3, breakpoint_handler);
    register_interrupt_handler(4, overflow_handler);
}

void interrupt_dispatcher(interrupt_frame_t *frame)
{
    if (frame->int_no >= MAX_GATES) {
        kprintf("Kernel panic: invalid interrupt number!\n");
        for (;;) {
            __asm__ volatile ("cli; hlt");
	}
    }

    if (interrupt_handlers[frame->int_no] != NULL) {
        interrupt_handler_t handler = interrupt_handlers[frame->int_no];
        handler(frame);
    } else {
        kprintf("Unhandled interrupt.\n");
    }
}
