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
  in al, 0x92
  or al, 00000010b
  out 0x92, al

gdt_start:

gdt_null:
    dq 0x0000000000000000

gdt_code:
    dq 0x00CF9A000000FFFF

gdt_data:
    dq 0x00CF92000000FFFF

gdt_end:

gdt_descriptor:
