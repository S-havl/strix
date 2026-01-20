[bits 16]                         ; Define 16 bits for assembly
[org 0x7C00]                      ; Start assembly at physical address 0x7C00

start:
    jmp 0x0000:_start             ; Normalize CS to 0x0000 using a far jump

_start:
    cli                           ; Disable interrupts
    cld                           ; Clear direction flag

    xor ax, ax
    mov ds, ax
    mov es, ax                    ; Initialize segments to 0x0000
    mov ss, ax
    mov sp, 0x7C00                ; Stack pointer to 0x7C00

    sti                           ; Enable interruptions

    mov [boot_drive], dl          ; Save the boot drive number

    xor ax, ax                    ; Register AX to 0x0000
    mov es, ax                    ; Extra segment to 0x0000
    mov bx, STAGE2_OFFSET         ; Move offset 0x1000 to BX register

    mov ah, 0x02                  ; Select subfuntion 0x02 in AH to read disk sectors
    mov al, 4                     ; Read 4 sections
    mov ch, 0                     ; Zero cylinder
    mov cl, 2                     ; Read 4 sectors starting from sector number 2
    mov dh, 0                     ; Head number zero
    mov dl, [boot_drive]          ; Load boot drive number into DL
    int 0x13                      ; Interrupt 0x13 for disk functions
    jc hang                       ; Jump to the suspension loop if transport is activated (if it fails)

    jmp 0x0000:STAGE2_OFFSET      ; Far jump to bootloader stage 2

boot_drive: db 0                  ; Reserve bytes for the boot drive
STAGE2_OFFSET equ 0x1000          ; Constant for offset 0x1000

hang:                             ; Hang loop tag
    cli                           ; Disable interrupts
    hlt                           ; Stop processor waiting for an external interrupt
    jmp hang                      ; Jump to the hang tag for the loop

times 510 - ($-$$) db 0           ; Fill the remaining bytes with zeros to reach 510 bytes
dw 0xAA55                         ; Signature in the last two bytes so the CPU knows it's a contract bootloader
