format elf

public _MEM_enable_paging
public _MEM_set_pagedirectory

use32

section ".text"

_MEM_enable_paging:
	mov eax,cr0
	or eax,0x80000000
	mov cr0,eax
	ret

_MEM_set_pagedirectory:
	mov eax,[esp+4]
	mov cr3,eax
	ret
