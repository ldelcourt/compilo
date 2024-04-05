.globl toto
.type toto, @function
toto: 
 	pushq %rbp
 	movq %rsp, %rbp
 	subq $32, %rsp
toto_principal:
 	jmp toto_bb0
toto_bb0:
 	mov %rdi, -4(%rbp)
 	mov %rsi, -8(%rbp)
 	mov %rdx, -12(%rbp)
 	movl -4(%rbp), %edx
 	movl -8(%rbp), %eax
 	addl %edx, %eax
 	movl %eax, -16(%rbp)
 	movl -16(%rbp), %edx
 	movl -12(%rbp), %eax
 	addl %edx, %eax
 	movl %eax, -20(%rbp)
 	movl -20(%rbp), %eax
 	jmp toto_output
toto_output:
 	movq %rbp, %rsp
 	popq %rbp
 	ret
.globl main
.type main, @function
main: 
 	pushq %rbp
 	movq %rsp, %rbp
 	subq $16, %rsp
main_principal:
 	jmp main_bb0
main_bb0:
 	mov $3, %rdx
 	mov $4, %rsi
 	mov $6, %rdi
 	call toto
 	movl %eax, -8(%rbp)
 	movl -8(%rbp), %eax
 	movl %eax, -4(%rbp)
 	movl -4(%rbp), %eax
 	jmp main_output
main_output:
 	movq %rbp, %rsp
 	popq %rbp
 	ret
