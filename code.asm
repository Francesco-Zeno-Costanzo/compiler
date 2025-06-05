;====================================================================;
; assembly translation of the code.txt code,                         ;
; it is reported just to check that everything works                 ;
;============================== Usage ===============================;
; nasm -f elf64 code.asm                                             ;
; ld -o code code.o -lc --dynamic-linker=/lib64/ld-linux-x86-64.so.2 ;
; ./code                                                             ;
;====================================================================;

section .data
format db "%d", 10, 0       ; format string for printf: "%d\n"
a dq 2                      ; variable a, initialized to 2 (64-bit)
b dq 4                      ; variable b, initialized to 4 (64-bit)

section .bss
c resq 1                    ; reserve 8 bytes for c (a + b)
d resq 1                    ; reserve 8 bytes for d (b - a)
e resq 1                    ; reserve 8 bytes for e (a * b)
f resq 1                    ; reserve 8 bytes for f (b / a)

section .text
extern printf               ; declare external function printf
global _start               ; entry point of the program

_start:
	mov rax, 2              ; rax = 2
	mov [a], rax            ; store 2 into a
	mov rax, 4              ; rax = 4
	mov [b], rax            ; store 4 into b

	mov rax, [a]            ; rax = a
	add rax, [b]            ; rax = a + b
	mov [c], rax            ; store result in c

	mov rax, [b]            ; rax = b
	sub rax, [a]            ; rax = b - a
	mov [d], rax            ; store result in d

	mov rax, [a]            ; rax = a
	imul rax, [b]           ; rax = a * b
	mov [e], rax            ; store result in e

	mov rax, [b]            ; rax = b
	cqo                     ; sign-extend rax into rdx:rax
	idiv qword [a]          ; rax = b / a (signed integer division)
	mov [f], rax            ; store result in f

	; print a
	mov rdi, format         ; first argument: format string
	mov rsi, [a]            ; second argument: value of a
	xor rax, rax            ; clear rax (required for variadic functions)
	call printf             ; call printf("%d\n", a)

	; print b
	mov rdi, format
	mov rsi, [b]
	xor rax, rax
	call printf

	; print c (a + b)
	mov rdi, format
	mov rsi, [c]
	xor rax, rax
	call printf

	; print d (b - a)
	mov rdi, format
	mov rsi, [d]
	xor rax, rax
	call printf

	; print e (a * b)
	mov rdi, format
	mov rsi, [e]
	xor rax, rax
	call printf

	; print f (b / a)
	mov rdi, format
	mov rsi, [f]
	xor rax, rax
	call printf

	; exit the program
	mov rax, 60             ; syscall number for exit
	xor rdi, rdi            ; exit code = 0
	syscall                 ; perform the syscall