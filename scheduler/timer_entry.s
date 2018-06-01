format elf

public _TIMER_ENTRY

section ".text"

use32

_TIMER_ENTRY:
	mov ax,[char]
	mov [0xb8000],ax
    inc ax
    mov [char], ax
	mov al,0x20
	out 0x20,al
	iret	

char:
	dw 0x0141
