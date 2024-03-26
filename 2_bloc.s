	.file	"2_bloc.c"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$96, %rsp
	movl	$3, -4(%rbp)
	movl	$5, -8(%rbp)
	movl	$5, -12(%rbp)
	movl	$5, -16(%rbp)
	movl	$5, -20(%rbp)
	movl	$5, -24(%rbp)
	movl	$5, -28(%rbp)
	movl	$5, -32(%rbp)
	movl	$5, -36(%rbp)
	movl	$5, -40(%rbp)
	movl	$5, -44(%rbp)
	movl	$5, -48(%rbp)
	movl	$5, -52(%rbp)
	movl	$5, -56(%rbp)
	movl	$5, -60(%rbp)
	movl	$5, -64(%rbp)
	movl	$5, -68(%rbp)
	movl	$5, -72(%rbp)
	movl	$5, -76(%rbp)
	movl	$5, -80(%rbp)
	movl	$5, -84(%rbp)
	movl	$5, -88(%rbp)
	movl	-4(%rbp), %eax
	movl	%eax, %edi
	call	putchar@PLT
	movl	-8(%rbp), %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
