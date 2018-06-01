format elf

public _PORT_BYTE_OUT
public _PORT_WORD_OUT
public _PORT_BYTE_IN
public _PORT_WORD_IN
public _PORT_DWORD_IN
public _PORT_DWORD_OUT

section ".text"

_PORT_BYTE_OUT:
	push ebp
	mov ebp,esp
	mov edx,[ebp+8]
	mov eax,[ebp+12]
	out dx,al
	mov esp,ebp
	pop ebp
	ret


_PORT_WORD_OUT:
	push ebp
	mov ebp,esp
	mov edx,[ebp+8]
	mov eax,[ebp+12]
	out dx,ax
	mov esp,ebp
	pop ebp
	ret 


_PORT_DWORD_OUT:
	push ebp
	mov ebp,esp
	mov edx,[ebp+8]
	mov eax,[ebp+12]
	out dx,eax
	mov esp,ebp
	pop ebp
	ret


_PORT_BYTE_IN:
	push ebp
	mov ebp,esp
	xor eax,eax
	mov edx,[ebp+8]
	in al,dx
	mov esp,ebp
	pop ebp
	ret


_PORT_WORD_IN:
	push ebp
	mov ebp,esp
	xor eax,eax
	mov edx,[ebp+8]
	in ax,dx
	mov esp,ebp
	pop ebp
	ret

_PORT_DWORD_IN:
	push ebp
	mov ebp,esp
	xor eax,eax
	mov edx,[ebp+8]
	in eax,dx
	mov esp,ebp
	pop ebp
	ret


