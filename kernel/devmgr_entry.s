format elf

extrn _DEVMGR_serve

public _DEVMGR_ENTRY

section ".text"

use32

_DEVMGR_ENTRY:
    push edx
    push eax
    call _DEVMGR_serve

_DEVMGR_END:
    pop eax
    pop edx
    iret
