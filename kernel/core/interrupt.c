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

// Handlers for idt gates 0-32
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

void bound_range_exceeded_handler(interrupt_frame_t *frame)
{
    kprintf("#BR\n");
}

void invalid_opcode_handler(interrupt_frame_t *frame)
{
    kprintf("#UD\n");
}

void device_not_available_handler(interrupt_frame_t *frame)
{
    kprintf("#NM\n");
}

void double_fault_handler(interrupt_frame_t *frame)
{
    kprintf("#DF\n");
}

void coproccesor_segment_overrun_handler(interrupt_frame_t *frame)
{
    kprintf("#Coproccesor segment overrun.\n");
}

void invalid_tss_handler(interrupt_frame_t *frame)
{
    kprintf("#TS\n");
}

void segment_not_present_handler(interrupt_frame_t *frame)
{
    kprintf("#NP\n");
}

void stack_segment_fault_handler(interrupt_frame_t *frame)
{
    kprintf("#SS\n");
}

void general_protection_handler(interrupt_frame_t *frame)
{
    kprintf("#GP\n");
}

void page_fault_handler(interrupt_frame_t *frame)
{
    kprintf("#PF\n");
}

void intel_reserved_do_not_use_handler(interrupt_frame_t *frame)
{
    kprintf("Intel reserved.\n");
}

void x87_fpu_floating_point_error_handler(interrupt_frame_t *frame)
{
    kprintf("MF\n");
}

void alignment_check_handler(interrupt_frame_t *frame)
{
    kprintf("AC\n");
}

void machine_check_handler(interrupt_frame_t *frame)
{
    kprintf("#MC\n");
}

void interrupt_handlers_init(void)
{
    register_interrupt_handler(0, divide_error_handler);
    register_interrupt_handler(1, debug_exception_handler);
    register_interrupt_handler(2, nmi_interrupt_handler);
    register_interrupt_handler(3, breakpoint_handler);
    register_interrupt_handler(4, overflow_handler);
    register_interrupt_handler(5, bound_range_exceeded_handler);
    register_interrupt_handler(6, invalid_opcode_handler);
    register_interrupt_handler(7, device_not_available_handler);
    register_interrupt_handler(8, double_fault_handler);
    register_interrupt_handler(9, coproccesor_segment_overrun_handler);
    register_interrupt_handler(10, invalid_tss_handler);
    register_interrupt_handler(11, segment_not_present_handler);
    register_interrupt_handler(12, stack_segment_fault_handler);
    register_interrupt_handler(13, general_protection_handler);
    register_interrupt_handler(14, page_fault_handler);
    register_interrupt_handler(15, intel_reserved_do_not_use_handler);
    register_interrupt_handler(16, x87_fpu_floating_point_error_handler);
    register_interrupt_handler(17, alignment_check_handler);
    register_interrupt_handler(18, machine_check_handler);
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
