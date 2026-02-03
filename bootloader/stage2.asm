; ==================================================
;                   STAGE 2
;           Protected Mode Bootloader
; ==================================================

[bits 16]
[org 0x1000]

stage2_start:
    cli
    cld

    ; Initialize segments and stack in RM
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x9C00

    mov [boot_drive], dl          ; Save boot drive number

    jmp enter_protect_mode        ; Transition to PM

; ==========================================================
;            ENTER PROTECTED MODE (RM -> PM)
; ==========================================================
enter_protect_mode:
    in al, 0x92
    or al, 00000010b              ; Enable A20 line
    out 0x92, al

    lgdt [gdt_descriptor]         ; Load GDT

    cli
    mov eax, cr0
    or eax, 1                     ; Enable PE bit
    mov cr0, eax

    jmp CODE_SEL:protected_entry  ; Far jump to 32-bit PM

; ---------------------------------------------
;                     GDT (32 BITS)
; ---------------------------------------------
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

; ==================================================
;                 PROTECTED MODE
; ==================================================
[bits 32]

protected_entry:
    ; Setup data segments and stack
    mov ax, DATA_SEL
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x9FC00

    ; Print status message
    mov esi, entering_pm_msg
    call print_entering_msg

    jmp setup_long_mode

; -------------------------------------------
; PRINT FUNCTION (32-bit PM)
; -------------------------------------------
print_entering_msg:
    pusha
    mov edi, VGA_MEMORY + (8*80 + 0) * 2

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

; ==================================================
;                SETUP LONG MODE
; ==================================================
setup_long_mode:
    mov eax, cr4
    or eax, 0x20
    mov cr4, eax

    lgdt [gdt64_descriptor]

    mov eax, pml4
    mov cr3, eax

    mov ecx, 0xC0000080
    rdmsr
    or eax, 0x100
    wrmsr

    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax

    jmp CODE64_SEL:long_mode_entry

; ==================================================
;                GDT (64 BITS)
; ==================================================

align 8
gdt64_start:
    dq 0
gdt64_code:
    dq 0x00AF9A000000FFFF
gdt64_data:
    dq 0x00AF92000000FFFF
gdt64_end:

gdt64_descriptor:
    dw gdt64_end - gdt64_start - 1
    dd gdt64_start

CODE64_SEL equ 1 << 3
DATA64_SEL equ 2 << 3

; ==================================================
;                PAGE TABLES (4 levels)
; ==================================================

align 4096
pml4:
    dq pdpt + 0x03

align 4096
pdpt:
    dq pd + 0x03

align 4096
pd:
    dq 0x00000000 + 0x83

; --------------------------------------------------
;                STACK (64 BITS)
; --------------------------------------------------

align 16
stack_bottom:
    times 16384 db 0
stack_top:

; ---------------- CONSTANTS / DATA PM ----------------
VGA_MEMORY equ 0xB8000
COLOR equ 0x0D
entering_pm_msg db "Entering long mode...", 0

; ==================================================
;                    LONG MODE
; ==================================================
[bits 64]

long_mode_entry:
    mov ax, DATA64_SEL
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov rsp, stack_top

    and rsp, -16

.hang:
    cli
    hlt
    jmp .hang
