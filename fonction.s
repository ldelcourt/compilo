.globl toto
.type toto, @function
toto: 
 	pushq %rbp
 	movq %rsp, %rbp
 	subq $64, %rsp
toto_principal:
 	jmp toto_bb0
toto_bb0:
 	movl %edi, -4(%rbp)
 	movl %esi, -8(%rbp)
 	movl %edx, -12(%rbp)
 	movl -4(%rbp), %edx
 	movl -8(%rbp), %eax
 	addl %edx, %eax
 	movl %eax, -16(%rbp)
 	movl -16(%rbp), %edx
 	movl -12(%rbp), %eax
 	addl %edx, %eax
 	movl %eax, -20(%rbp)
 	movl -20(%rbp), %eax
 	cmpl $81, %eax
 	setg	  %al
 	movzbl	%al, %eax
 	movl %eax, -24(%rbp)
 	cmpl $0, -24(%rbp)
 	je toto_bb2
 	jmp toto_bb1
toto_output:
 	movq %rbp, %rsp
 	popq %rbp
 	ret
toto_bb1:
 	movl -4(%rbp), %edx
 	movl -8(%rbp), %eax
 	addl %edx, %eax
 	movl %eax, -28(%rbp)
 	movl -28(%rbp), %edx
 	movl -12(%rbp), %eax
 	addl %edx, %eax
 	movl %eax, -32(%rbp)
 	movl -32(%rbp), %eax
 	jmp toto_bb3
toto_bb2:
 	movl -12(%rbp), %edx
 	movl $1, %eax
 	addl %edx, %eax
 	movl %eax, -36(%rbp)
 	movl -8(%rbp), %edx
 	movl $1, %eax
 	addl %edx, %eax
 	movl %eax, -40(%rbp)
 	movl -4(%rbp), %edx
 	movl $1, %eax
 	addl %edx, %eax
 	movl %eax, -44(%rbp)
 	movl -36(%rbp), %edx
 	movl -40(%rbp), %esi
 	movl -44(%rbp), %edi
 	call toto
 	movl %eax, -48(%rbp)
 	movl -48(%rbp), %eax
 	jmp toto_bb3
toto_bb3:
 	jmp toto_output
.globl main
.type main, @function
main: 
 	pushq %rbp
 	movq %rsp, %rbp
 	subq $16, %rsp
main_principal:
 	jmp main_bb0
main_bb0:
 	movl $3, %edx
 	movl $4, %esi
 	movl $6, %edi
 	call toto
 	movl %eax, -12(%rbp)
 	movl -12(%rbp), %eax
 	movl %eax, -4(%rbp)
 	movl -4(%rbp), %eax
 	movl %eax, -8(%rbp)
 	movl -8(%rbp), %eax
 	jmp main_output
main_output:
 	movq %rbp, %rsp
 	popq %rbp
 	ret
