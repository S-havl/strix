#include <stdint.h>
#include <kprintf.h>

typedef struct {
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
    uint64_t rdi, rsi, rbp, rdx, rcx, rbx, rax;

    uint64_t int_no;
    uint64_t err_code;

    uint64_t rip, cs, rflags, rsp, ss;
} __attribute__((packed)) interrupt_frame_t;

static volatile uint16_t* const vga = (volatile uint16_t*)0xB8000;

void interrupt_dispatcher(interrupt_frame_t *frame)
{
    switch (frame->int_no) {
        case 0:
            vga[0] = 0x4F55;
        case 1:
            vga[2] = 0x4F57;
        case 2:
            vga[4] = 0x4F55;
    }
}
