; ========================================================
; READ E820 MEMORY MAP
; ========================================================

e820_start:
	xor	ax, ax
	mov	es, ax
	mov	di, 0x6004
	xor	ebx, ebx
	xor	bp, bp
	mov	edx, 0x534D4150
	mov	eax, 0xe820
	mov	dword [es:di + 20], 1
	mov	ecx, 24
	int	0x15
	jc	short .failed
	mov	edx, 0x534D4150
	cmp	eax, edx
	jne	short .failed
	test	ebx, ebx
	je	short .failed
	jmp	short .jmpin

.e820_loop:
	mov	eax, 0xe820
	mov	dword [es:di + 20], 1
	mov	ecx, 24
	int	0x15
	jc	short .e820f
	mov	edx, 0x534D4150

.jmpin:
	jcxz	.skipent
	cmp	cl, 20
	jbe	short .notext
	test	byte [es:di + 20], 1
	je	short .skipent

.notext:
	mov	eax, dword [es:di + 8]
	mov	ecx, dword [es:di + 12]
	or	eax, ecx
	jz	.skipent
	inc	bp
	add	di, 24

.skipent:
	test	ebx, ebx
	jne	short .e820_loop

.e820f:
	mov	[0x6002], bp
	clc
	ret

.failed:
	stc
	ret

