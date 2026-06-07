; ==========================================================
; SET VBE MODE 1024x768x32 
; ==========================================================

	mov	ax, 0x4F01
	mov	cx, 0x0118
	mov	di, vbe_mode_info_block
	
	push	ds
	pop	es
	
	int	0x10

	mov	ax, 0x4F02
	mov	bx, 0x4118
	int	0x10

	mov	eax, [vbe_mode_info_block + 0x28]
	mov	[framebuffer_phys], eax

	mov	ax, [vbe_mode_info_block + 0x12]
	mov	[screen_width], ax

	mov	ax, [vbe_mode_info_block + 0x14]
	mov	[screen_height], ax

	mov	ax, [vbe_mode_info_block + 0x10]
	mov	[screen_pitch], ax
