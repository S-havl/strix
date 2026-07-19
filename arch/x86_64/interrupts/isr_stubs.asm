section	.text

global	isr0
global	isr1
global	isr2
global	isr3
global	isr_common

extern	interrupt_dispatcher

isr0:
	push	qword	0
	push	qword	0
	jmp	isr_common

isr1:
	push	qword	0
	push	qword	1
	jmp	isr_common

isr2:
	push	qword	0
	push	qword	2
	jmp	isr_common

isr3:
	push	qword	0
	push	qword	3
	jmp	isr_common

isr_common:
	cld

	push	rax
	push	rbx
	push	rcx
	push	rdx
	push	rbp
	push	rsi
	push	rdi

	push	r8
	push	r9
	push	r10
	push	r11
	push	r12
	push	r13
	push	r14
	push	r15

	mov	rdi, rsp
	mov	rbp, rsp
	and	rsp, -16

	call	interrupt_dispatcher

	mov	rsp, rbp

	pop	r15
	pop	r14
	pop	r13
	pop	r12
	pop	r11
	pop	r10
	pop	r9
	pop	r8

	pop	rdi
	pop	rsi
	pop	rbp
	pop	rdx
	pop	rcx
	pop	rbx
	pop	rax

	add	rsp, 16

	iretq

global	isr_stub_table

section .data
align	8
isr_stub_table:
	dq	isr0
	dq	isr1
	dq	isr2
	dq	isr3
