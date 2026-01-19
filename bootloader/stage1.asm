[bits 16]                         ; Define 16 bits for assembly
[org 0x7C00]                      ; Start assembly at physical address 0x7C00

start:
    jmp 0x0000:_start             ; Normalize CS to 0x0000 using a far jump

_start:
    cli
    cld

    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    sti

    mov [boot_drive], dl

    xor ax, ax
    mov es, ax
    mov bx, STAGE2_OFFSET

    mov ah, 0x02
    mov al, 4
    mov ch, 0
    mov cl, 2
    mov dh, 0
    mov dl, [boot_drive]
    int 0x13
    jc hang

    jmp 0x0000:STAGE2_OFFSET

boot_drive: db 0
STAGE2_OFFSET equ 0x1000

hang:
    cli
    hlt
    jmp hang

times 510 - ($-$$) db 0
dw 0xAA55
