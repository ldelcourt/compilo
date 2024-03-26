.globl main
 main: 
 	pushq %rbp
 	movq %rsp, %rbp
 	subq $32, %rsp
 	call getchar
 	movl %eax, -12(%rbp)
 	movl $26, %eax
 	imull -12(%rbp), %eax
 	movl %eax, -16(%rbp)
 	movl -16(%rbp), %edx
 	movl $9, %eax
 	addl %edx, %eax
 	movl %eax, -20(%rbp)
 	movl -20(%rbp), %eax
 	movl %eax, -4(%rbp)
 	mov -4(%rbp), %rdi
 	call putchar
 	call getchar
 	movl %eax, -24(%rbp)
 	movl -24(%rbp), %eax
 	movl %eax, -8(%rbp)
 	mov -8(%rbp), %rdi
 	call putchar
 	movl -4(%rbp), %eax
 	subl -8(%rbp), %eax
 	movl %eax, -28(%rbp)
 	movl -28(%rbp), %eax
 	movl %eax, -4(%rbp)
 	mov -4(%rbp), %rdi
 	call putchar
 	movl -4(%rbp), %eax
 	movq %rbp, %rsp
 	popq %rbp
 	ret
