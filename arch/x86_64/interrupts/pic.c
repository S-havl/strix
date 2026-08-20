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

#define OFFSET1 0x20
#define OFFSET2 0x28

static uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__ __volatile__(
        "inb %1, %0"
        : "=a"(ret)
        : "Nd"(port)
    );
    return ret;
}

static void outb(uint16_t port, uint8_t val)
{
    __asm__ __volatile__(
        "outb %0, %1"
        :
        : "a"(val), "Nd"(port)
    );
}

static inline void io_wait(void)
{
    __asm__ __volatile__ ("outb %0, $0x80" : : "a"((uint8_t)0));
}

void pic_init(void)
{
    __asm__ __volatile__ ("cli" : : : "memory");

    // uint8_t mask1 = inb(PICMASTER_DATA); off
    // uint8_t mask2 = inb(PICSLAVE_DATA); off

    outb(PICMASTER_CMD, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PICSLAVE_CMD, ICW1_INIT | ICW1_ICW4);
    io_wait();

    outb(PICMASTER_DATA, OFFSET1);
    io_wait();
    outb(PICSLAVE_DATA, OFFSET2);
    io_wait();

    outb(PICMASTER_DATA, 0x04);
    io_wait();
    outb(PICSLAVE_DATA, 0x02);
    io_wait();

    outb(PICMASTER_DATA, ICW4_8086);
    io_wait();
    outb(PICSLAVE_DATA, ICW4_8086);
    io_wait();

    outb(PICMASTER_DATA, 0xFF); // off = mask1
    outb(PICSLAVE_DATA, 0xFF);  // off = mask2
}
