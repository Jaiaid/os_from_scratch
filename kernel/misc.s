format elf

public _ENABLE_A20

section ".text"

use32

kb_wait:
    in al, 0x64
    test al, 0x2
    jnz kb_wait
    ret

kb_wait1:
    in al, 0x60
    test al, 0x1
    jz kb_wait1
    ret

_ENABLE_A20:
    call kb_wait
    mov al, 0xad
    out 0x64, al

    call kb_wait
    mov al, 0xd0
    out 0x64, al

    call kb_wait1
    in al, 0x60
    push eax

    call kb_wait
    mov al, 0xd1
    out 0x64, al

    call kb_wait
    pop eax
    or al, 0x2
    out 0x60, al

    call kb_wait
    mov al, 0xae
    out 0x64, al

    call kb_wait
    ret
