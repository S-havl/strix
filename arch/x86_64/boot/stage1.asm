; ==================================================
;                   STAGE 1
;          Real Mode Bootloader Entry
; ==================================================

[bits 16]
[org 0x7C00]

start:
	jmp	0x0000:_start	; Normalize CS to 0x0000 using a far jump

_start:
	cli	; Disable interrupts
	cld	; Clear direction flag

	; Initialize segments and stack
	xor	ax, ax
	mov	ds, ax
	mov	es, ax
	mov 	ss, ax
	mov	sp, 0x7C00

	sti	; Enable interrupts

	mov	[boot_drive], dl	; Save the boot drive number

	; Load Stage 2 from disk
	xor	ax, ax
	mov	es, ax
	mov	bx, STAGE2_OFFSET	; Offset 0x1000

	mov	ah, 0x02	; BIOS read sectors
	mov	al, 8	; Read 8 sectors
	mov	ch, 0	; Cylinder 0
	mov	cl, 2	; Starting sector 2
	mov	dh, 0	; Head 0
	mov	dl, [boot_drive]
	int	0x13	; Call BIOS
	jc	hang	; Hang if read fails

	jmp	0x0000:STAGE2_OFFSET	; Far jump to Stage 2

; ---------------- VARIABLES ----------------
boot_drive:	db	0
STAGE2_OFFSET	equ	0x1000

; --------------------------------------
;                 HANG
; --------------------------------------
hang:
	cli
.hang_loop:
	hlt
	jmp	.hang_loop

; ---------------- BOOT SIGNATURE ----------------
	times	510 - ($-$$)	db 0
	dw	0xAA55

