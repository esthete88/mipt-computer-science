	.file	"2.c"
	.text
	.globl	sort
	.type	sort, @function
sort:
.LFB0:
	pushq	%rbp
	movq	%rsp, %rbp
	movq	%rdi, -8(%rbp)
	movl	%esi, -12(%rbp)

	cmpl $0, %esi
	je end; 

	xor %rcx, %rcx

	label:
	addq $1, %rcx

	movq (%rdi, %rcx, 8), %r9
	movq %rcx, %r8
	subq $1, %r8

	while:
	cmpq $(-1), %r8
	jg end_while
	cmpq (%rdi, %r8, 8), %r9
	jng end_while
	movq (%rdi, %r8, 8), %rax
	movq %rax, 8(%rdi, %r8, 8)
	subq $1, %r8
	jmp while

	end_while:

	movq %r9, 8(%rdi, %r8, 8)

	cmpq %rsi, %rcx
	jg label



	end:
	xor %rax, %rax
	popq	%rbp
	ret
.LFE0:
	.size	sort, .-sort
	.ident	"GCC: (Ubuntu 4.8.4-2ubuntu1~14.04.3) 4.8.4"
	.section	.note.GNU-stack,"",@progbits
