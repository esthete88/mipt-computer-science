	.file	"sort.c"
	.text
	.globl	sort
	.type	sort, @function
sort:
.LFB0:
	pushq	%rbp
	movq	%rsp, %rbp
	movq	%rdi, -8(%rbp)
	movl	%esi, -12(%rbp)
	popq	%rbp
	ret
.LFE0:
	.size	sort, .-sort
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
