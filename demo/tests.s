	.file	"tests.cpp"
	.text
<<<<<<< HEAD
	.globl	d
	.data
	.align 4
	.type	d, @object
	.size	d, 4
d:
	.long	100
	.text
	.globl	_Z3sumii
	.type	_Z3sumii, @function
_Z3sumii:
=======
	.globl	_Z4funcR1A
	.type	_Z4funcR1A, @function
_Z4funcR1A:
>>>>>>> 2fbd3f96eabdcb7a359c5544e9757e9546bc908a
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
<<<<<<< HEAD
	movl	%edi, -20(%rbp)
	movl	%esi, -24(%rbp)
	movq	$0, -8(%rbp)
	addq	$4, -8(%rbp)
	movl	d(%rip), %eax
	cltq
	addq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
=======
	movq	%rdi, -8(%rbp)
	movl	$1, %eax
>>>>>>> 2fbd3f96eabdcb7a359c5544e9757e9546bc908a
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
<<<<<<< HEAD
	.size	_Z3sumii, .-_Z3sumii
=======
	.size	_Z4funcR1A, .-_Z4funcR1A
>>>>>>> 2fbd3f96eabdcb7a359c5544e9757e9546bc908a
	.globl	main
	.type	main, @function
main:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
<<<<<<< HEAD
	subq	$16, %rsp
	movl	$101010, -4(%rbp)
	movl	-4(%rbp), %edx
	movl	-4(%rbp), %eax
	movl	%edx, %esi
	movl	%eax, %edi
	call	_Z3sumii
	nop
=======
	subq	$48, %rsp
	movl	$1, -4(%rbp)
	movl	$2, -8(%rbp)
	movq	$3, -48(%rbp)
	movq	$4, -40(%rbp)
	movq	$5, -32(%rbp)
	leaq	-48(%rbp), %rax
	movq	%rax, %rdi
	call	_Z4funcR1A
	movl	%eax, -12(%rbp)
	movl	$0, %eax
>>>>>>> 2fbd3f96eabdcb7a359c5544e9757e9546bc908a
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.ident	"GCC: (GNU) 8.3.1 20191121 (Red Hat 8.3.1-5)"
	.section	.note.GNU-stack,"",@progbits
