section .text
global	isr0

isr0:
	mov	byte [0xB8000], 'X'
	mov	byte [0xB8001], 0x4F

isr_common:
