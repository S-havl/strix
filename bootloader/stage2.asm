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

    mov esi, entering_pm_msg      ; Move the message to be printed to the ESI log
    call print_entering_msg       ; Jump to the label or function print_entering_msg

hang:                             ; Hang loop tag
    cli                           ; Disable interrupts
    hlt                           ; Stop processor waiting for an external interrupt
    jmp hang                      ; Jump to the hang tag for the loop

print_entering_msg:               ; Function label
    pusha                         ; Save the current state of all general-purpose records
    mov edi, VGA_MEMORY + (8 * 80 + 0) * 2       ; Move VGA MEMORY 0xB8000 to the EDI register, adding the print start location

.loop:                            ; Print loop label
    lodsb                         ; Load string byte
    test al, al                   ; Check if AL equals zero
    jz .done                      ; If AL equals zero, jump to the .done label/function

    mov ah, COLOR                 ; Move color 0x0D to AH
    mov [edi], ax                 ; Move the color along with the character from AX to the EDI record
    add edi, 2                    ; Add 2 to the EDI record to avoid overwriting characters
    jmp .loop                     ; Jump to the label to create the loop until the entire sentence is printed

.done:                            ; .done tag for when the entire sentence finishes printing
    popa                          ; Extract the state of the general-purpose registers we store on the stack with PUSHA
    ret                           ; Return to the point after the print_entering_msg call

VGA_MEMORY equ 0xB8000            ; Constant for VGA MEMORY 0xB8000
COLOR equ 0x0D                    ; Constant for attribute color
entering_pm_msg db "Entering long mode...", 0    ; Define memory for the message to be printed
