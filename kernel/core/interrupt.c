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
    kprintf(COLOR_YELLOW, COLOR_RED, "#DE\n");
    __asm__ volatile ("cli; hlt");
}

static void debug_exception_handler(interrupt_frame_t *frame)
{
    kprintf(COLOR_YELLOW, COLOR_RED, "#DB\n");
    __asm__ volatile ("cli; hlt");
}

static void nmi_interrupt_handler(interrupt_frame_t *frame)
{
    kprintf(COLOR_YELLOW, COLOR_RED, "NMI\n");
    __asm__ volatile ("cli; hlt");
}

static void breakpoint_handler(interrupt_frame_t *frame)
{
    kprintf(COLOR_YELLOW, COLOR_RED, "#BP\n");
    __asm__ volatile ("cli; hlt");
}

static void overflow_handler(interrupt_frame_t *frame)
{
    kprintf(COLOR_YELLOW, COLOR_RED, "#OF\n");
    __asm__ volatile ("cli; hlt");
}

static void bound_range_exceeded_handler(interrupt_frame_t *frame)
{
    kprintf(COLOR_YELLOW, COLOR_RED, "#BR\n");
    __asm__ volatile ("cli; hlt");
}

static void invalid_opcode_handler(interrupt_frame_t *frame)
{
    kprintf(COLOR_YELLOW, COLOR_RED, "#UD\n");
    __asm__ volatile ("cli; hlt");
}

static void device_not_available_handler(interrupt_frame_t *frame)
{
    kprintf(COLOR_YELLOW, COLOR_RED, "#NM\n");
    __asm__ volatile ("cli; hlt");
}

static void double_fault_handler(interrupt_frame_t *frame)
{
    kprintf(COLOR_YELLOW, COLOR_RED, "#DF\n");
    __asm__ volatile ("cli; hlt");
}

static void coproccesor_segment_overrun_handler(interrupt_frame_t *frame)
{
    kprintf(COLOR_YELLOW, COLOR_RED, "#Coproccesor segment overrun.\n");
    __asm__ volatile ("cli; hlt");
}

static void invalid_tss_handler(interrupt_frame_t *frame)
{
    kprintf(COLOR_YELLOW, COLOR_RED, "#TS\n");
    __asm__ volatile ("cli; hlt");
}

static void segment_not_present_handler(interrupt_frame_t *frame)
{
    kprintf(COLOR_YELLOW, COLOR_RED, "#NP\n");
    __asm__ volatile ("cli; hlt");
}

static void stack_segment_fault_handler(interrupt_frame_t *frame)
{
    kprintf(COLOR_YELLOW, COLOR_RED, "#SS\n");
    __asm__ volatile ("cli; hlt");
}

static void general_protection_handler(interrupt_frame_t *frame)
{
    kprintf(COLOR_YELLOW, COLOR_RED, "#GP\n");
    __asm__ volatile ("cli; hlt");
}

static void page_fault_handler(interrupt_frame_t *frame)
{
    kprintf(COLOR_YELLOW, COLOR_RED, "#PF\n");
    __asm__ volatile ("cli; hlt");
}

static void intel_reserved_do_not_use_handler(interrupt_frame_t *frame)
{
    kprintf(COLOR_YELLOW, COLOR_RED, "Intel reserved.\n");
    __asm__ volatile ("cli; hlt");
}

static void x87_fpu_floating_point_error_handler(interrupt_frame_t *frame)
{
    kprintf(COLOR_YELLOW, COLOR_RED, "MF\n");
    __asm__ volatile ("cli; hlt");
}

static void alignment_check_handler(interrupt_frame_t *frame)
{
    kprintf(COLOR_YELLOW, COLOR_RED, "AC\n");
    __asm__ volatile ("cli; hlt");
}

static void machine_check_handler(interrupt_frame_t *frame)
{
    kprintf(COLOR_YELLOW, COLOR_RED, "#MC\n");
    __asm__ volatile ("cli; hlt");
}

static void simd_floating_point_exception_handler(interrupt_frame_t *frame)
{
    kprintf(COLOR_YELLOW, COLOR_RED, "#XM\n");
    __asm__ volatile ("cli; hlt");
}

static void virtualization_exception_handler(interrupt_frame_t *frame)
{
    kprintf(COLOR_YELLOW, COLOR_RED, "#VE\n");
    __asm__ volatile ("cli; hlt");
}

static void control_protection_exception_handler(interrupt_frame_t *frame)
{
    kprintf(COLOR_YELLOW, COLOR_RED, "#CP\n");
    __asm__ volatile ("cli; hlt");
}

static void reserved_vector_22_handler(interrupt_frame_t *frame)
{
    kprintf(COLOR_YELLOW, COLOR_RED, "#N/A22\n");
    __asm__ volatile ("cli; hlt");
}

static void reserved_vector_23_handler(interrupt_frame_t *frame)
{
    kprintf(COLOR_YELLOW, COLOR_RED, "#N/A23\n");
    __asm__ volatile ("cli; hlt");
}

static void reserved_vector_24_handler(interrupt_frame_t *frame)
{
    kprintf(COLOR_YELLOW, COLOR_RED, "#N/A24\n");
    __asm__ volatile ("cli; hlt");
}

static void reserved_vector_25_handler(interrupt_frame_t *frame)
{
    kprintf(COLOR_YELLOW, COLOR_RED, "#N/A25\n");
    __asm__ volatile ("cli; hlt");
}

static void reserved_vector_26_handler(interrupt_frame_t *frame)
{
    kprintf(COLOR_YELLOW, COLOR_RED, "#N/A26\n");
    __asm__ volatile ("cli; hlt");
}

static void reserved_vector_27_handler(interrupt_frame_t *frame)
{
    kprintf(COLOR_YELLOW, COLOR_RED, "#N/A27\n");
    __asm__ volatile ("cli; hlt");
}

static void reserved_vector_28_handler(interrupt_frame_t *frame)
{
    kprintf(COLOR_YELLOW, COLOR_RED, "#N/A28\n");
    __asm__ volatile ("cli; hlt");
}

static void reserved_vector_29_handler(interrupt_frame_t *frame)
{
    kprintf(COLOR_YELLOW, COLOR_RED, "#N/A29\n");
    __asm__ volatile ("cli; hlt");
}

static void reserved_vector_30_handler(interrupt_frame_t *frame)
{
    kprintf(COLOR_YELLOW, COLOR_RED, "#N/A30\n");
    __asm__ volatile ("cli; hlt");
}

static void reserved_vector_31_handler(interrupt_frame_t *frame)
{
    kprintf(COLOR_YELLOW, COLOR_RED, "#N/A31\n");
    __asm__ volatile ("cli; hlt");
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
    register_interrupt_handler(24, reserved_vector_24_handler);
    register_interrupt_handler(25, reserved_vector_25_handler);
    register_interrupt_handler(26, reserved_vector_26_handler);
    register_interrupt_handler(27, reserved_vector_27_handler);
    register_interrupt_handler(28, reserved_vector_28_handler);
    register_interrupt_handler(29, reserved_vector_29_handler);
    register_interrupt_handler(30, reserved_vector_30_handler);
    register_interrupt_handler(31, reserved_vector_31_handler);
}

void interrupt_dispatcher(interrupt_frame_t *frame)
{
    if (frame->int_no >= MAX_GATES) {
        kprintf(COLOR_YELLOW, COLOR_RED, "Kernel panic: invalid interrupt number!\n");
        for (;;) {
            __asm__ __volatile__ ("cli; hlt");
        }
    }

    if (interrupt_handlers[frame->int_no] != NULL) {
        interrupt_handler_t handler = interrupt_handlers[frame->int_no];
        handler(frame);
    } else {
        kprintf(COLOR_YELLOW, COLOR_BLACK, "Unhandled interrupt.\n");
    }
}
