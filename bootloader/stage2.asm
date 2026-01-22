[bits 16]                         ; Define 16 bits for assembly
[org 0x1000]                      ; Start assembly at 0x1000

stage2_start:
    cli                           ; Disable interrupts
    cld                           ; Clear direction flag

    xor ax, ax
    mov ds, ax
    mov es, ax                    ; Initialize segments to 0x0000
    mov ss, ax
    mov sp, 0x9C00                ; Stack pointer to 0x9C00

    mov [boot_drive], dl          ; Save the boot drive number

    in al, 0x92
    or al, 00000010b              ; Enable A20 reading and writing to port 0x92 by modifying bit 1
    out 0x92, al

    lgdt [gdt_descriptor]         ; Load the GDT into the GDTR register with LGDT

    cli                           ; Ensure interruptions are disabled (Do not assume)
    mov eax, cr0
    or eax, 1                     ; Activating protect mode (PM) by copying the value from CR0 to EAX,
    mov cr0, eax                  ; turning on bit 1 with OR, and returning the value to CR0

    jmp CODE_SEL:protected_entry  ; Far jump to the label where the 32-bit protect mode (PM) starts

gdt_start:
                                  ; Creating the GDT table:
gdt_null:
    dq 0x0000000000000000         ; Null descriptor (0)

gdt_code:
    dq 0x00CF9A000000FFFF         ; Code descriptor (1)

gdt_data:
    dq 0x00CF92000000FFFF         ; Data descriptor (2)

gdt_end:

gdt_descriptor:                   ; Creating the GDT descriptor
    dw gdt_end - gdt_start - 1    ; GDT Size - 1
    dd gdt_start                  ; Address where the GDT begins

CODE_SEL equ 1 << 3               ; Selector constant for code descriptor (0x08)
DATA_SEL equ 2 << 3               ; Selector constant for the data descriptor (0x10)

boot_drive: db 0                  ; Reserve bytes for the boot drive

[bits 32]                         ; Define 32 bits for assembly

protected_entry:                  ; Protectted mode (PM) is initiated
    mov ax, DATA_SEL
    mov ds, ax
    mov es, ax
    mov fs, ax                    ; Initialize segments with the DATA_SEL selector using the GDT table
    mov gs, ax
    mov ss, ax
    mov esp, 0x9FC00              ; Stack pointer to 0x9FC00

    mov esi, entering_pm_msg
    call print_entering_msg

hang:                             ; Hang loop tag
    cli                           ; Disable interrupts
    hlt                           ; Stop processor waiting for an external interrupt
    jmp hang                      ; Jump to the hang tag for the loop

print_entering_msg:
    pusha
    mov edi, VGA_MEMORY + (8 * 80 + 0) * 2

.loop:
    lodsb
    test al, al
    jz .done

    mov ah, COLOR
    mov [edi], ax
    add edi, 2
    jmp .loop

.done:
    popa
    ret

VGA_MEMORY equ 0xB8000
COLOR equ 0x0D
entering_pm_msg db "Entering long mode...", 0
