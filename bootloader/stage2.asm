[bits 16]
[org 0x1000]

boot_drive: db 0
kernel_addr: 0x2000

stage2_start:
    cli
    cld

    mov ax, 0x1000
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x9C00

    mov [boot_drive], dl

enable_a20:

gdt_start:

gdt_null:

gdt_code:

gdt_data:

gdt_end:

gdt_descriptor:
