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
    mov sp, 0x9000

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
    mov esp, 0x9000

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

    mov edi, 0x2000
    mov dword [edi], 0x3000 | 0x03
    mov dword [edi+4], 0

    mov edi, 0x3000
    mov dword [edi], 0x4000 | 0x03
    mov dword [edi+4], 0

    mov edi, 0x4000

    mov eax, 0x00000083   ; 0–2MB
    mov [edi], eax
    mov dword [edi+4], 0

    mov eax, 0x00200083   ; 2–4MB
    mov [edi+8], eax
    mov dword [edi+12], 0

    mov eax, 0x00400083   ; 4–6MB
    mov [edi+16], eax
    mov dword [edi+20], 0

    mov eax, 0x00600083   ; 6–8MB
    mov [edi+24], eax
    mov dword [edi+28], 0

    mov eax, 0x00800083   ; 8–10MB
    mov [edi+32], eax
    mov dword [edi+36], 0

    mov eax, 0x00A00083   ; 10–12MB
    mov [edi+40], eax
    mov dword [edi+44], 0

    mov eax, 0x00C00083   ; 12–14MB
    mov [edi+48], eax
    mov dword [edi+52], 0

    mov eax, 0x00E00083   ; 14–16MB
    mov [edi+56], eax
    mov dword [edi+60], 0

    mov edi, 0x5000

    mov dword [edi], 0
    mov dword [edi+4], 0

    mov dword [edi+8], 0x00000000
    mov dword [edi+12], 0x00209A00

    mov dword [edi+16], 0x00000000
    mov dword [edi+20], 0x00009200

    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    mov eax, 0x2000
    mov cr3, eax

    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    lgdt [gdt64_descriptor]

    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax

    jmp 0x08:long_mode_entry

gdt64_descriptor:
    dw 24-1
    dd 0x5000

; ---------------- CONSTANTS / DATA PM ----------------
VGA_MEMORY equ 0xB8000
COLOR equ 0x0D
entering_pm_msg db "Entering long mode...", 0

; ==================================================
;                    LONG MODE
; ==================================================
[bits 64]

long_mode_entry:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov rsp, 0x90000
    and rsp, -16

.hang:
    cli
    hlt
    jmp .hang
