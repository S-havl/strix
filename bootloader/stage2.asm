[bits 16]
[org 0x1000]

stage2_start:
    cli
    cld

    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x9C00

    mov [boot_drive], dl

    in al, 0x92
    or al, 00000010b
    out 0x92, al

    lgdt [gdt_descriptor]

    cli
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp CODE_SEL:protected_entry

gdt_start:

gdt_null:
    dq 0x0000000000000000

gdt_code:
    dq 0x00CF9A000000FFFF

gdt_data:
    dq 0x00CF92000000FFFF

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEL equ 1 << 3
DATA_SEL equ 2 << 3

boot_drive: db 0

[bits 32]

protected_entry:
    mov ax, DATA_SEL
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x9FC00

    mov edi, 0xB8000
    add edi, (8*80 + 0)*2
    mov ax, 0x0D4F
    mov [edi], ax

    mov ax, 0x0D4B
    mov [edi + 2], ax

hang:
    cli
    hlt
    jmp hang
