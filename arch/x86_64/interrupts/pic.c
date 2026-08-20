#include <arch/x86_64/interrupts/pic.h>

#include <stdint.h>
#include <stddef.h>

#define PICMASTER_CMD 0x20
#define PICMASTER_DATA 0x21

#define PICSLAVE_CMD 0xA0
#define PICSLAVE_DATA 0xA1

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__ __volatile__(
        "inb %1, %0"
        : "=a"(ret)
        : "Nd"(port)
    );
    return ret;
}

void outb(uint16_t port, uint8_t val)
{
    __asm__ __volatile__(
        "outb %0, %1"
        :
        : "a"(val), "Nd"(port)
    );
}

void pic_init(void)
{
    // pic remap here
}
