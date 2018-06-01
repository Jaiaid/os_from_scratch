format elf


extrn _DISPLAYDRIVER_ENTRY1_printChar
extrn _DISPLAYDRIVER_ENTRY2_nextLine
extrn _DISPLAYDRIVER_ENTRY3_lineFirst

public _DISPLAYDRIVER_ENTRY

section ".text"

use32

_DISPLAYDRIVER_ENTRY:
	cmp eax,2
	ja _DISPLAYDRIVER_END
	cmp eax,0
	je _DISPLAYDRIVER_ENTRY1
	cmp eax,1
	je _DISPLAYDRIVER_ENTRY2
	cmp eax,2
	je _DISPLAYDRIVER_ENTRY3


_DISPLAYDRIVER_ENTRY1:
    push edx
    call _DISPLAYDRIVER_ENTRY1_printChar
    pop edx
    jmp _DISPLAYDRIVER_END

_DISPLAYDRIVER_ENTRY2:
    push edx
    call _DISPLAYDRIVER_ENTRY2_nextLine
    pop edx
    jmp _DISPLAYDRIVER_END

_DISPLAYDRIVER_ENTRY3:
    push edx
    call _DISPLAYDRIVER_ENTRY3_lineFirst
    pop edx

_DISPLAYDRIVER_END:
	iret
