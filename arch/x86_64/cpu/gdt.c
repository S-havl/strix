#include <stdint.h>
#include <arch/x86_64/cpu/gdt.h>
#include <arch/x86_64/cpu/tss.h>

#define GDT_SIZE 7

#define GDT_FLAG_GRANULARITY 0x80
#define GDT_FLAG_LONG        0x20

#define GDT_FLAGS_CODE (GDT_FLAG_GRANULARITY | GDT_FLAG_LONG)
#define GDT_FLAGS_DATA (GDT_FLAG_GRANULARITY)

#pragma pack(push, 1)
struct GDTEntry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_high;
};

struct GDTR {
    uint16_t limit;
    uint64_t base;
};
#pragma pack(pop)

static struct GDTEntry gdt[GDT_SIZE] __attribute__((aligned(8)));
static struct GDTR gdtr;
void gdt_flush(void *gdtr_ptr);


static void set_gdt_entry(struct GDTEntry *entry, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags) {
    *entry = (struct GDTEntry){0};

    entry->base_low    = base & 0xFFFF;
    entry->base_middle = (base >> 16) & 0xFF;
    entry->base_high   = (base >> 24) & 0xFF;
    entry->limit_low   = limit & 0xFFFF;
    entry->granularity = ((limit >> 16) & 0x0F) | (flags & 0xF0);
    entry->access      = access;
}


static void set_tss_descriptor(int index, uint64_t base, uint32_t limit) {
    uint64_t *gdt64 = (uint64_t*)gdt;

    uint64_t low = 0;
    uint64_t high = 0;

    low |= (limit & 0xFFFF);
    low |= (base & 0xFFFFFF) << 16;
    low |= (uint64_t)0x89 << 40;
    low |= ((uint64_t)((limit >> 16) & 0xF)) << 48;
    low |= ((uint64_t)((base >> 24) & 0xFF)) << 56;

    high = (uint64_t)(base >> 32);

    gdt64[index]     = low;
    gdt64[index + 1] = high;
}

void gdt_init(void) {
    gdtr.limit = sizeof(gdt) - 1;
    gdtr.base  = (uint64_t)&gdt;

    set_gdt_entry(&gdt[0], 0, 0, 0, 0);

    set_gdt_entry(&gdt[1], 0, 0xFFFFF, 0x9A, GDT_FLAGS_CODE);
    set_gdt_entry(&gdt[2], 0, 0xFFFFF, 0x92, GDT_FLAGS_DATA);

    set_gdt_entry(&gdt[3], 0, 0xFFFFF, 0xFA, GDT_FLAGS_CODE);
    set_gdt_entry(&gdt[4], 0, 0xFFFFF, 0xF2, GDT_FLAGS_DATA);

    set_tss_descriptor(5, (uint64_t)&tss, sizeof(tss) - 1);

    gdt_flush(&gdtr);

    // far jump
    /*
    asm volatile (
        "pushq $0x08\n"
	"lea 1f(%%rip), %%rax\n"
	"pushq %%rax\n"
	"lretq\n"
	"1:\n"
	::: "rax", "memory"
    );
    

    asm volatile ("" ::: "memory");
    

    tss_flush(0x28);
    */

}
