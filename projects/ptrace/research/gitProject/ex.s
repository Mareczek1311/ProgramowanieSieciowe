	.file	"ex.c"
	.text
	.globl	ret
	.type	ret, @function
ret:
.LFB11:
	.cfi_startproc
	movl	$69, %eax
	ret
	.cfi_endproc
.LFE11:
	.size	ret, .-ret
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"siema "
	.text
	.globl	main
	.type	main, @function
main:
.LFB12:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movl	$.LC0, %edi
	call	puts
	movl	$0, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE12:
	.size	main, .-main
	.ident	"GCC: (GNU) 8.5.0 20210514 (Red Hat 8.5.0-19)"
	.section	.note.GNU-stack,"",@progbits
