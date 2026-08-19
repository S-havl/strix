#include <arch/x86_64/interrupts/pic.h>

#include <stdint.h>
#include <stddef.h>

#define PICMASTER_CMD 0x20
#define PICMASTER_DATA 0x21

#define PICSLAVE_CMD 0xA0
#define PICSLAVE_DATA 0xA1

void pic_init(void)
{
    // pic remap here
}
