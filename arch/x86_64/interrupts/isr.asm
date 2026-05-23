global	isr_stub

section	.text

isr_stub:
	cli

.hang:
	hlt
	jmp	.hang
