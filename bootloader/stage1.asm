[bits 16]
[org 0x7C00]

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

    mov ah, 0x02
    mov al, 4
    mov ch, 0
    mov cl, 2
    mov dh, 0
    mov dl, [boot_drive]
    mov bx, [stage2_addr]
    int 0x13

    mov ax, [stage2_addr]
    jmp ax

boot_drive: db 0
stage2_addr: dw 0x1000

hang:
    cli
    hlt
    jmp hang

times 510 - ($-$$) db 0
dw 0xAA55
