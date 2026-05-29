global	isr_stub

section	.text

isr_stub:
	mov	edi, 0x01000000
	mov	dword [edi], 0x00FFFFFF

.hang:
	hlt
	jmp	.hang
