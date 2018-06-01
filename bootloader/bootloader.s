CODE_SEG_SELECTOR equ 0x08
RM_SCREEN_COLOR equ 0x0b00  ;2nd byte indicates color
BOOT_SIGNATURE equ  0xaa55
LOAD_START_OFFSET equ 0x7e00
KERNEL_OFFSET equ 0x8000
LOADED_NUM_SECTOR equ 0x0027

org 0x7c00

BOOTLOADER:
    xor ax,ax
    mov ds,ax
    mov ss,ax
    mov sp,0xfffe
    mov bp,sp
    xor dx,dx              ;dh= row dl=column
    xor bx,bx              ;bh = page number
    mov ah,2               ;setting cursor
    int 0x10               ;setting cursor to initial of page 0    
	call CLR_SCRN    
    push MSG1
    call PUTS_RM
    add sp,2 
    push ds
    push LOAD_START_OFFSET
    push 0x0080
    push 0x0002
    push LOADED_NUM_SECTOR
    call  LOAD_DISK
    cmp ax,LOADED_NUM_SECTOR
    jne DISK_ERROR_EXCEPTION
    add sp,10
    push MSG2
    call PUTS_RM
    lgdt [GDT_DESCRIPTOR]
	cli
	mov eax,cr0
    or eax,0x1
    mov cr0,eax
	jmp CODE_SEG_SELECTOR:KERNEL_OFFSET
    jmp $
    
DISK_ERROR_EXCEPTION:
    push DISK_ERROR_MSG
    call PUTS_RM
    add sp,2
    jmp $
    
PUTS_RM:
    push bp
    mov bp,sp
    mov bx,[bp+4]
    mov ah,0xe
    PUTS_RM_L1:
        mov al,byte [bx]
        test al,0xff
        jz  PUTS_RM_L1_END
        int 0x10
        inc bx
        jmp PUTS_RM_L1
    PUTS_RM_L1_END:
    pop bp
    ret

CLR_SCRN:
    push bp
    mov bp,sp
    push ds                                                   ;ds is stored because 0xb8000 is far more than 64k from 0x7c00
    mov ax,0xb800
    mov ds,ax
    mov ax,RM_SCREEN_COLOR
    xor bx,bx    
    CLR_SCRN_L1:
        mov [bx],ax
        inc bx
        inc bx
        cmp bx,4000                                     ;screen has 80x25 character cell for each 2 byte
        jne CLR_SCRN_L1
    CLR_SCRN_L1_END:
    pop ds
    pop bp
    ret            

LOAD_DISK:
    push bp
    mov bp,sp
    mov ax,[bp+4]
    mov cx,[bp+6]
    mov dx,[bp+8]
    mov bx,[bp+10]
    mov es,[bp+12]
    or ax,0x0200
    int 0x13
    pop bp    
    ret


MSG1: 
    db "BOOT SUCCESSFUL",13,10,0

DISK_ERROR_MSG:
    db "DISK READ FAILED",13,10,0

MSG2:
    db "DISK LOAD SUCCESSFUL",13,10,0

times 510-($-$$) db 0
dw BOOT_SIGNATURE

include "../gdt/gdt.s"

times 1024-($-$$) db 0
