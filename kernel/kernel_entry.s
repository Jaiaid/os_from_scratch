format elf

CODE_SEG_SELECTOR equ 0x08
DATA_SEG_SELECTOR equ 0x10

extrn main

section ".text"

use32

_KERNEL_ENTRY:
    mov eax,DATA_SEG_SELECTOR
	mov ds,ax
    mov ss,ax
    mov fs,ax
    mov es,ax
    mov gs,ax
	mov esp,0x7fffe
	cli
	call main
	mov eax, 0x1205019
    jmp $


