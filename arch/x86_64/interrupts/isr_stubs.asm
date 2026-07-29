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

isr4:
	push	qword	0
	push	qword	4
	jmp	isr_common

isr5:
	push	qword	0
	push	qword	5
	jmp	isr_common

isr6:
	push	qword	0
	push	qword	6
	jmp	isr_common

isr7:
	push	qword	0
	push	qword	7
	jmp	isr_common

isr8:
	push	qword	0	; Delete in production
	push	qword	8
	jmp	isr_common

isr9:
	push	qword	0
	push	qword	9
	jmp	isr_common

isr10:
	push	qword	0	; Delete in production
	push	qword	10
	jmp	isr_common

isr11:
	push	qword	0	; Delete in production
	push	qword	11
	jmp	isr_common

isr12:
	push	qword	0	; Delete in production
	push	qword	12
	jmp	isr_common

isr13:
	push	qword	0	; Delete in production
	push	qword	13
	jmp	isr_common

isr14:
	push	qword	0	; Delete in production
	push	qword	14
	jmp	isr_common

isr15:
	push	qword	0	; Delete in production
	push	qword	15
	jmp	isr_common

isr16:
	push	qword	0
	push	qword	16
	jmp	isr_common

isr17:
	push	qword	0	; Delete in production
	push	qword	17
	jmp	isr_common

isr18:
	push	qword	0
	push	qword	18
	jmp	isr_common

isr19:
	push	qword	0
	push	qword	19
	jmp	isr_common

isr20:
	push	qword	0
	push	qword	20
	jmp	isr_common

isr21:
	push	qword	0	; Delete in production
	push	qword	21
	jmp	isr_common

isr22:
	push	qword	0
	push	qword	22
	jmp	isr_common

isr23:
	push	qword	0
	push	qword	23
	jmp	isr_common

isr24:
	push	qword	0
	push	qword	24
	jmp	isr_common

isr25:
	push	qword	0
	push	qword	25
	jmp	isr_common

isr26:
	push	qword	0
	push	qword	26
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
	dq	isr4
	dq	isr5
	dq	isr6
	dq	isr7
	dq	isr8
	dq	isr9
	dq	isr10
	dq	isr11
	dq	isr12
	dq	isr13
	dq	isr14
	dq	isr15
	dq	isr16
	dq	isr17
	dq	isr18
	dq	isr19
	dq	isr20
	dq	isr21
	dq	isr22
	dq	isr23
	dq	isr24
	dq	isr25
	dq	isr26
