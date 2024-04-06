.globl main
.type main, @function
main: 
 	pushq %rbp
 	movq %rsp, %rbp
 	subq $16, %rsp
main_principal:
 	jmp main_bb0
main_bb0:
 	movl $101, %edi
 	call putchar
 	jmp main_output
main_output:
 	movq %rbp, %rsp
 	popq %rbp
 	ret
