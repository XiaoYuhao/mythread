	.file	"context.cpp"
	.local	_ZStL8__ioinit
	.comm	_ZStL8__ioinit,1,1
	.text
	.globl	_Z11subfunctionv
	.type	_Z11subfunctionv, @function
_Z11subfunctionv:
.LFB971:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE971:
	.size	_Z11subfunctionv, .-_Z11subfunctionv
	.section	.rodata
.LC0:
	.string	"run this"
	.text
	.globl	_Z8functionv
	.type	_Z8functionv, @function
_Z8functionv:
.LFB972:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$.LC0, %esi
	movl	$_ZSt4cout, %edi
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	movl	$_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_, %esi
	movq	%rax, %rdi
	call	_ZNSolsEPFRSoS_E
	movl	$1, %edi
	call	sleep
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE972:
	.size	_Z8functionv, .-_Z8functionv
	.section	.rodata
.LC1:
	.string	"I am yuhaoxiao."
	.text
	.globl	_Z9function2v
	.type	_Z9function2v, @function
_Z9function2v:
.LFB973:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$.LC1, %esi
	movl	$_ZSt4cout, %edi
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	movl	$_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_, %esi
	movq	%rax, %rdi
	call	_ZNSolsEPFRSoS_E
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE973:
	.size	_Z9function2v, .-_Z9function2v
	.section	.rodata
.LC2:
	.string	"in main"
.LC3:
	.string	"test flag 1"
	.text
	.globl	main
	.type	main, @function
main:
.LFB974:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$264976, %rsp
	movl	$.LC2, %esi
	movl	$_ZSt4cout, %edi
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	movl	$_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_, %esi
	movq	%rax, %rdi
	call	_ZNSolsEPFRSoS_E
	leaq	-262144(%rbp), %rax
	movl	$131072, %edx
	movl	$0, %esi
	movq	%rax, %rdi
	call	memset
	leaq	-131072(%rbp), %rax
	movl	$131072, %edx
	movl	$0, %esi
	movq	%rax, %rdi
	call	memset
	leaq	-264976(%rbp), %rax
	movq	%rax, %rdi
	call	getcontext
	leaq	-263088(%rbp), %rax
	movq	%rax, %rdi
	call	getcontext
	leaq	-131072(%rbp), %rax
	movq	%rax, -263072(%rbp)
	movq	$131072, -263056(%rbp)
	movl	$0, -263064(%rbp)
	leaq	-264032(%rbp), %rax
	movq	%rax, -263080(%rbp)
	leaq	-263088(%rbp), %rax
	movl	$0, %edx
	movl	$_Z9function2v, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	makecontext
	leaq	-262144(%rbp), %rax
	movq	%rax, -264960(%rbp)
	movq	$131072, -264944(%rbp)
	movl	$0, -264952(%rbp)
	leaq	-263088(%rbp), %rax
	movq	%rax, -264968(%rbp)
	leaq	-264976(%rbp), %rax
	movl	$0, %edx
	movl	$_Z8functionv, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	makecontext
	movl	$.LC3, %esi
	movl	$_ZSt4cout, %edi
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	movl	$_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_, %esi
	movq	%rax, %rdi
	call	_ZNSolsEPFRSoS_E
	leaq	-264976(%rbp), %rdx
	leaq	-264032(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	swapcontext
	movl	$.LC2, %esi
	movl	$_ZSt4cout, %edi
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	movl	$_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_, %esi
	movq	%rax, %rdi
	call	_ZNSolsEPFRSoS_E
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE974:
	.size	main, .-main
	.type	_Z41__static_initialization_and_destruction_0ii, @function
_Z41__static_initialization_and_destruction_0ii:
.LFB983:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	cmpl	$1, -4(%rbp)
	jne	.L8
	cmpl	$65535, -8(%rbp)
	jne	.L8
	movl	$_ZStL8__ioinit, %edi
	call	_ZNSt8ios_base4InitC1Ev
	movl	$__dso_handle, %edx
	movl	$_ZStL8__ioinit, %esi
	movl	$_ZNSt8ios_base4InitD1Ev, %edi
	call	__cxa_atexit
.L8:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE983:
	.size	_Z41__static_initialization_and_destruction_0ii, .-_Z41__static_initialization_and_destruction_0ii
	.type	_GLOBAL__sub_I__Z11subfunctionv, @function
_GLOBAL__sub_I__Z11subfunctionv:
.LFB984:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$65535, %esi
	movl	$1, %edi
	call	_Z41__static_initialization_and_destruction_0ii
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE984:
	.size	_GLOBAL__sub_I__Z11subfunctionv, .-_GLOBAL__sub_I__Z11subfunctionv
	.section	.init_array,"aw"
	.align 8
	.quad	_GLOBAL__sub_I__Z11subfunctionv
	.hidden	__dso_handle
	.ident	"GCC: (GNU) 4.8.5 20150623 (Red Hat 4.8.5-36)"
	.section	.note.GNU-stack,"",@progbits
