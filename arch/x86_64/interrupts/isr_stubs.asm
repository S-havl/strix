section	.text

global	isr0
global	isr_common

isr0:
	mov	byte [0xB8000], 'X'
	mov	byte [0xB8001], 0x4F

isr_common:
	push	r15
	push	r14
	push	r13
	push	r12
	push	r11
	push	r10
	push	r9
	push	r8

	push	rdi
	push	rsi
	push	rbp
	push	rdx
	push	rcx
	push	rbx
	push	rax

	mov	rax, rsp
	and	rsp, -16

	mov	rdi, rsp
;	call	interrupt

	mov	rsp, rax

	pop	rax
	pop	rbx
	pop	rcx
	pop	rdx
	pop	rbp
	pop	rsi
	pop	rdi

	pop	r8
	pop	r9
	pop	r10
	pop	r11
	pop	r12
	pop	r13
	pop	r14
	pop	r15

	add	rsp, 16

	iretq
