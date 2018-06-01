CODE_SEG_SELECTOR equ 0x8
DATA_SEG_SELECTOR equ 0x10

GDT:
    
    GDT_DESCRIPTOR:
        dw GDT_END-GDT-1
        dd GDT
        dw 0x0000

    CODE_SEG:
        dw 0xffff
        dw 0x0000
        db 0x0
        db 10011010b
        db 11001111b
        db 0x0
    
    DATA_SEG:
        dw 0xffff
        dw 0x0000
        db 0x0
        db 10010010b
        db 11001111b
        db 0x0

GDT_END:
