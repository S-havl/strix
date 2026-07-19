#pragma once

#include <arch/x86_64/interrupts/interrupt_frame.h>

typedef void (*interrupt_handler_t)(interrupt_frame_t *frame);
void interrupt_handlers_init(void);
