#include <kernel/core/interrupt.h>
#include <arch/x86_64/interrupts/interrupt_frame.h>

#include <stdint.h>
#include <stddef.h>
#include <kprintf.h>

#define MAX_GATES 256

interrupt_handler_t interrupt_handlers[MAX_GATES] = {NULL};

static void register_interrupt_handler(uint8_t n, interrupt_handler_t handler)
{
    interrupt_handlers[n] = handler;
}

// Handlers for idt gates 0-32
static void divide_error_handler(interrupt_frame_t *frame)
{
    kprintf("#DE\n");
}

static void debug_exception_handler(interrupt_frame_t *frame)
{
    kprintf("#DB\n");
}

static void nmi_interrupt_handler(interrupt_frame_t *frame)
{
    kprintf("NMI\n");
}

static void breakpoint_handler(interrupt_frame_t *frame)
{
    kprintf("#BP\n");
}

static void overflow_handler(interrupt_frame_t *frame)
{
    kprintf("#OF\n");
}

static void bound_range_exceeded_handler(interrupt_frame_t *frame)
{
    kprintf("#BR\n");
}

static void invalid_opcode_handler(interrupt_frame_t *frame)
{
    kprintf("#UD\n");
}

static void device_not_available_handler(interrupt_frame_t *frame)
{
    kprintf("#NM\n");
}

static void double_fault_handler(interrupt_frame_t *frame)
{
    kprintf("#DF\n");
}

static void coproccesor_segment_overrun_handler(interrupt_frame_t *frame)
{
    kprintf("#Coproccesor segment overrun.\n");
}

static void invalid_tss_handler(interrupt_frame_t *frame)
{
    kprintf("#TS\n");
}

static void segment_not_present_handler(interrupt_frame_t *frame)
{
    kprintf("#NP\n");
}

static void stack_segment_fault_handler(interrupt_frame_t *frame)
{
    kprintf("#SS\n");
}

static void general_protection_handler(interrupt_frame_t *frame)
{
    kprintf("#GP\n");
}

static void page_fault_handler(interrupt_frame_t *frame)
{
    kprintf("#PF\n");
}

static void intel_reserved_do_not_use_handler(interrupt_frame_t *frame)
{
    kprintf("Intel reserved.\n");
}

static void x87_fpu_floating_point_error_handler(interrupt_frame_t *frame)
{
    kprintf("MF\n");
}

static void alignment_check_handler(interrupt_frame_t *frame)
{
    kprintf("AC\n");
}

static void machine_check_handler(interrupt_frame_t *frame)
{
    kprintf("#MC\n");
}

static void simd_floating_point_exception_handler(interrupt_frame_t *frame)
{
    kprintf("#XM\n");
}

static void virtualization_exception_handler(interrupt_frame_t *frame)
{
    kprintf("#VE\n");
}

static void control_protection_exception_handler(interrupt_frame_t *frame)
{
    kprintf("#CP\n");
}

static void reserved_vector_22_handler(interrupt_frame_t *frame)
{
    kprintf("#N/A\n");
}

static void reserved_vector_23_handler(interrupt_frame_t *frame)
{
    kprintf("#N/A\n");
}

static void reserved_vector_24_handler(interrupt_frame_t *frame)
{
    kprintf("#N/A\n");
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
    register_interrupt_handler(19, simd_floating_point_exception_handler);
    register_interrupt_handler(20, virtualization_exception_handler);
    register_interrupt_handler(21, control_protection_exception_handler);
    register_interrupt_handler(22, reserved_vector_22_handler);
    register_interrupt_handler(23, reserved_vector_23_handler);
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
