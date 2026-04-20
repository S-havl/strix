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
	xor	ax, ax
	mov	ds, ax
	mov	es, ax
	mov	ss, ax
	mov	sp, 0x9000
	
	mov	[boot_drive], dl	; Save boot drive number
	
	mov	ax, 0x0000
	mov	es, ax
	mov	bx, 0x8000
	
	mov	ah, 0x02
	mov	al, 128
	mov	ch, 0
	mov	cl, 11
	mov	dh, 0
	mov	dl, [boot_drive]
	int	0x13
	
	jmp	enter_protect_mode	; Transition to PM

; ==========================================================
;            ENTER PROTECTED MODE (RM -> PM)
; ==========================================================
enter_protect_mode:
	in	al, 0x92
	or	al, 00000010b		; Enable A20 line
	out	0x92, al
	
	lgdt	[gdt_descriptor]	; Load GDT
	
	cli
	mov	eax, cr0
	or	eax, 1			; Enable PE bit
	mov	cr0, eax
	
	jmp	CODE_SEL:protected_entry	; Far jump to 32-bit PM

; ---------------------------------------------
;                     GDT (32 BITS)
; ---------------------------------------------
gdt_start:
gdt_null:
	dq	0x0000000000000000
gdt_code:
	dq	0x00CF9A000000FFFF
gdt_data:
	dq	0x00CF92000000FFFF
gdt_end:

gdt_descriptor:
	dw	gdt_end - gdt_start - 1
	dd	gdt_start

CODE_SEL	equ	1 << 3
DATA_SEL	equ	2 << 3

boot_drive:	db	0

; ==================================================
;                 PROTECTED MODE
; ==================================================
[bits 32]

protected_entry:
					; Setup data segments and stack
	mov	ax, DATA_SEL
	mov	ds, ax
	mov	es, ax
	mov	fs, ax
	mov	gs, ax
	mov	ss, ax
	mov	esp, 0x9000

	mov	esi, entering_lm_msg
	call	print_entering_msg

	mov	esi, 0x8000
	call	check_elf
	
	add	esi, 0x18
	mov	eax, [esi]
	call	check_entry_point
	
	jmp	setup_long_mode

check_elf:
	pusha
	
	mov	eax, [esi]
	cmp	eax, 0x464C457F
	jne	.not_elf
	
	mov	esi, elf_ok_msg
	call	print_entering_msg
	jmp	.done

.not_elf:
	mov	esi, elf_fail_msg
	call	print_entering_msg

.done:
	popa
	ret

check_entry_point:
	pusha
	
	mov	edi, entry_buffer
	mov	ecx, 8
hex_loop:
	mov	edx, eax
	shr	edx, 28
	and	edx, 0xF
	
	cmp	dl, 9
	jle	.is_number
	add	dl, 55
	jmp	.store

.is_number:
	add	dl, 48

.store:
	mov	[edi], dl
	inc	edi
	
	shl	eax, 4
	loop	hex_loop
	
	mov	byte [edi], 0
	
	mov	esi, entry_buffer
	call	print_entering_msg
	
	popa
	ret


; -------------------------------------------
; PRINT FUNCTION (32-bit PM)
; -------------------------------------------
print_entering_msg:
	pusha
	mov	edi, [cursor_pos]

.loop:
	lodsb
	test	al, al
	jz	.done
	mov	ah, COLOR
	mov	[edi], ax
	add	edi, 2
	jmp	.loop

.done:
	add	dword [cursor_pos], 160
	
	popa
	ret

; ==================================================
;                SETUP LONG MODE
; ==================================================
setup_long_mode:

	mov	edi, 0x2000
	mov	dword [edi], 0x3000 | 0x03
	mov	dword [edi+4], 0
	
	mov	edi, 0x3000
	mov	dword [edi], 0x4000 | 0x03
	mov	dword [edi+4], 0
	
	mov	edi, 0x4000
	
	mov	eax, 0x00000083   ; 0–2MB
	mov	[edi], eax
	mov	dword [edi+4], 0
	
	mov	eax, 0x00200083   ; 2–4MB
	mov	[edi+8], eax
	mov	dword [edi+12], 0
	
	mov	eax, 0x00400083   ; 4–6MB
	mov	[edi+16], eax
	mov	dword [edi+20], 0
	
	mov	eax, 0x00600083   ; 6–8MB
	mov	[edi+24], eax
	mov	dword [edi+28], 0
	
	mov	eax, 0x00800083   ; 8–10MB
	mov	[edi+32], eax
	mov	dword [edi+36], 0
	
	mov	eax, 0x00A00083   ; 10–12MB
	mov	[edi+40], eax
	mov	dword [edi+44], 0
	
	mov	eax, 0x00C00083   ; 12–14MB
	mov	[edi+48], eax
	mov	dword [edi+52], 0
	
	mov	eax, 0x00E00083   ; 14–16MB
	mov	[edi+56], eax
	mov	dword [edi+60], 0
	
	mov	edi, 0x5000
	
	mov	dword [edi], 0
	mov	dword [edi+4], 0
	
	mov	dword [edi+8], 0x00000000
	mov	dword [edi+12], 0x00209A00
	
	mov	dword [edi+16], 0x00000000
	mov	dword [edi+20], 0x00009200

	mov	eax, cr4
	or	eax, 1 << 5
	mov	cr4, eax
	
	mov	eax, 0x2000
	mov	cr3, eax
	
	mov	ecx, 0xC0000080
	rdmsr
	or	eax, 1 << 8
	wrmsr
	
	lgdt	[gdt64_descriptor]
	
	mov	eax, cr0
	or	eax, 0x80000000
	mov	cr0, eax
	
	jmp	0x08:long_mode_entry

gdt64_descriptor:
	dw	24-1
	dd	0x5000

; ---------------- CONSTANTS / DATA PM ----------------
VGA_MEMORY	equ	0xB8000
COLOR		equ	0x0A
entering_lm_msg	db	"Entering long mode...", 0
elf_ok_msg	db	"ELF successfully detected. Entry point: 0x", 0
elf_fail_msg	db	"The file is not ELF :C", 0
entry_buffer	times	9	db	0
cursor_pos	dd	VGA_MEMORY + (8*80)*2

; ==================================================
;                    LONG MODE
; ==================================================
[bits 64]

long_mode_entry:
	cli
	cld
	
	mov	ax, 0x10
	mov	ds, ax
	mov	es, ax
	mov	fs, ax
	mov	gs, ax
	mov	ss, ax
	
	mov	rsp, 0x90000
	and	rsp, -16
	
	mov	rbx, ELF64
	
	movzx	r8, word [rbx + E_PHNUM]	; e_phnum
	movzx	r9, word [rbx + E_PHENTSIZE]	; e_phentsize
	
	mov	rdx, [rbx + E_PHOFF]		; e_phoff
	lea	rdx, [rbx + rdx]		; absolute address e_phoff

.loop:
	test	r8, r8
	jz	.end
	
	mov	eax, dword [rdx]	; p_type
	cmp	eax, 1			; PT_LOAD
	jne	.next
	
	mov	r10, [rdx + P_OFFSET]	; p_offset
	mov	r11, [rdx + P_VADDR]	; p_vaddr
	mov	r12, [rdx + P_FILESZ]	; p_filesz
	mov	r13, [rdx + P_MEMSZ]	; p_memsz
	
	lea	rsi, [rbx + r10]
	mov	rdi, r11
	mov	rcx, r12
	rep	movsb
	
	mov	rcx, r13
	sub	rcx, r12
	jz	.next
	xor	al, al
	rep	stosb

.next:
	add	rdx, r9
	dec	r8
	jmp	.loop

.end:
	mov	rax, [rbx + 0x18]
	jmp	rax

.hang:
	cli
	hlt
	jmp .hang

; ---------------- CONSTANTS / DATA LM ----------------
ELF64		equ	0x8000
E_PHOFF		equ	0x20
E_PHENTSIZE	equ	0x36
E_PHNUM		equ	0x38
P_OFFSET	equ	0x08
P_VADDR		equ	0x10
P_FILESZ	equ	0x20
P_MEMSZ		equ	0x28










