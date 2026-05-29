global	isr_stub

section	.text

isr_stub:
	mov	byte [0xB8000], 'X'
	mov	byte [0xB8001], 0x4F

.hang:
	hlt
	jmp	.hang
