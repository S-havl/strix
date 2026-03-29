#include <arch/x86_64/cpu/tss.h>

struct TSS tss __attribute__((aligned(16))) = {0};
