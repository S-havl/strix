#include <stdint.h>
#include <stddef.h>
#include <kprintf.h>

#define MAX_GATES 256

typedef struct {
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
    uint64_t rdi, rsi, rbp, rdx, rcx, rbx, rax;

    uint64_t int_no;
    uint64_t err_code;

    uint64_t rip, cs, rflags, rsp, ss;
} __attribute__((packed)) interrupt_frame_t;

typedef void (*interrupt_handler_t)(interrupt_frame_t *frame);
interrupt_handler_t interrupt_handlers[MAX_GATES] = {NULL};

void register_isr_handler(uint8_t n, interrupt_handler_t handler)
{
    isr_handlers[n] = handler;
}

void interrupt_dispatcher(interrupt_frame_t *frame)
{
    if (frame->int_no >= MAX_GATES) {
        kprintf("Kernel panic: invalid interrupt number\n");
	return;
    }

    if (interrupt_handlers[frame->int_no] != NULL) {
        interrupt_handler_t handler = interrupt_handlers[frame->int_no];
        handler(frame);
    } else {
        kprintf("Unhandled interrupt\n");
    }
}
