	.file	"pthread_exit.c"
	.comm	foo,16,16
.globl foo1
	.data
	.align 16
	.type	foo1, @object
	.size	foo1, 16
foo1:
	.long	1
	.long	2
	.long	3
	.long	5
	.section	.rodata
.LC0:
	.string	"pointer:%p\n"
.LC1:
	.string	"a:%d\n"
.LC2:
	.string	"b:%d\n"
.LC3:
	.string	"c:%d\n"
.LC4:
	.string	"d:%d\n"
	.text
.globl print_foo
	.type	print_foo, @function
print_foo:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	puts
	movl	$.LC0, %eax
	movq	-16(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf
	movq	-16(%rbp), %rax
	movl	(%rax), %edx
	movl	$.LC1, %eax
	movl	%edx, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf
	movq	-16(%rbp), %rax
	movl	4(%rax), %edx
	movl	$.LC2, %eax
	movl	%edx, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf
	movq	-16(%rbp), %rax
	movl	8(%rax), %edx
	movl	$.LC3, %eax
	movl	%edx, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf
	movq	-16(%rbp), %rax
	movl	12(%rax), %edx
	movl	$.LC4, %eax
	movl	%edx, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	print_foo, .-print_foo
	.section	.rodata
.LC5:
	.string	"thread 1:\n"
	.text
.globl thread_func1
	.type	thread_func1, @function
thread_func1:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movl	$1, -16(%rbp)
	movl	$2, -12(%rbp)
	movl	$3, -8(%rbp)
	movl	$4, -4(%rbp)
	movl	$foo1, %esi
	movl	$.LC5, %edi
	call	print_foo
	movl	$foo1, %edi
	call	pthread_exit
	.cfi_endproc
.LFE1:
	.size	thread_func1, .-thread_func1
	.section	.rodata
.LC6:
	.string	"thread2's ID is %d\n"
	.text
.globl thread_func2
	.type	thread_func2, @function
thread_func2:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	call	pthread_self
	movq	%rax, %rdx
	movl	$.LC6, %eax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf
	movl	$1, %edi
	call	pthread_exit
	.cfi_endproc
.LFE2:
	.size	thread_func2, .-thread_func2
	.section	.rodata
.LC7:
	.string	"error while creating thread"
.LC8:
	.string	"error while join thread 1"
.LC9:
	.string	"error while creating thread2"
.LC10:
	.string	"Parent:\n"
	.text
.globl main
	.type	main, @function
main:
.LFB3:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movl	%edi, -36(%rbp)
	movq	%rsi, -48(%rbp)
	leaq	-16(%rbp), %rax
	movl	$0, %ecx
	movl	$thread_func1, %edx
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_create
	movl	%eax, -4(%rbp)
	cmpl	$0, -4(%rbp)
	je	.L8
	movl	$.LC7, %edi
	call	puts
	movl	$1, %eax
	jmp	.L9
.L8:
	leaq	-32(%rbp), %rdx
	movq	-16(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	pthread_join
	movl	%eax, -4(%rbp)
	cmpl	$0, -4(%rbp)
	je	.L10
	movl	$.LC8, %edi
	call	puts
	movl	$2, %eax
	jmp	.L9
.L10:
	leaq	-24(%rbp), %rax
	movl	$0, %ecx
	movl	$thread_func2, %edx
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_create
	movl	%eax, -4(%rbp)
	cmpl	$0, -4(%rbp)
	je	.L11
	movl	$.LC9, %edi
	call	puts
	movl	$3, %eax
	jmp	.L9
.L11:
	movl	$1, %edi
	call	sleep
	movq	-32(%rbp), %rax
	movq	%rax, %rsi
	movl	$.LC10, %edi
	call	print_foo
	movl	$0, %eax
.L9:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	main, .-main
	.ident	"GCC: (GNU) 4.4.7 20120313 (Red Hat 4.4.7-3)"
	.section	.note.GNU-stack,"",@progbits
