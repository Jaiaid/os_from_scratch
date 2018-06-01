format elf

extrn	_KEYBOARDDRIVER_ENTRY1_READCHAR

public _KEYBOARDDRIVER_ENTRY
public _SCS1_TO_ASCII_MAP

section ".text"

use32

_KEYBOARDDRIVER_ENTRY:
	cmp eax,1
	jae _KEYBOARDDRIVER_END
	jmp _KEYBOARDDRIVER_ENTRY1

_KEYBOARDDRIVER_ENTRY1:
	call _KEYBOARDDRIVER_ENTRY1_READCHAR
	jmp _KEYBOARDDRIVER_END	

_KEYBOARDDRIVER_END:
	iret


_SCS1_TO_ASCII_MAP:
    db 0x0,0x0,"1234567890-=",0x8,0x9
    db "qwertyuiop[]",0xd,0x0,"as"
    db "dfghjkl;'`",0x0,"\zxcv"
    db "bnm,./",0x0,0x0,0x0,' ',0x0,0x0,0x0,0x0,0x0,0x0
    db 0x0,0x0,0x0,0x0,0x0,0x0,0x0
    times 256-($-_SCS1_TO_ASCII_MAP) db 0
