	.text
	.globl	sort
	.type	sort, @function
sort:
	pushq	%rbp
	movq	%rsp, %rbp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rsi, %r9
	subq	$1, %r9
	xor %rcx, %rcx
	for1:
		movq 0(%rdi,%rcx,8), %rax
		movq %rcx, %rdx
		movq %rcx, %r8
		incq %rdx
		for2:
			cmpq %rax, 0(%rdi,%rdx,8)
				jnl endfor2
				movq %rdx, %r8
				movq 0(%rdi,%rdx,8), %rax
			endfor2: incq %rdx
				cmpq %rdx, %rsi
					jne for2
		xchg %rax, 0(%rdi,%rcx,8)
		movq %rax, 0(%rdi,%r8,8)
		incq %rcx
		cmpq %rcx, %r9
			jne for1
	popq	%rbp
	xor %rax, %rax
	retq
