	.file	"lab2_1.c"
	.text
	.globl	fib
	.type	fib, @function
fib:
.LFB0:
	.cfi_startproc
	testl	%edi, %edi
	je	.L4
	cmpl	$1, %edi
	je	.L5
	movl	$1, %esi
	movl	$1, %ecx
	movl	$0, %edx
	jmp	.L3
.L6:
	movl	%eax, %ecx
.L3:
	leal	(%rsi,%rcx), %eax
	addl	$1, %edx
	movl	%ecx, %esi
	cmpl	%edi, %edx
	jne	.L6
	rep ret
.L4:
	movl	$0, %eax
	ret
.L5:
	movl	$0, %eax
.L2:
	.p2align 4,,2
	ret
	.cfi_endproc
.LFE0:
	.size	fib, .-fib
	.ident	"GCC: (Ubuntu 4.8.5-4ubuntu8) 4.8.5"
	.section	.note.GNU-stack,"",@progbits
