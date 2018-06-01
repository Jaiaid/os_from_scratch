#ifndef PORT_IO_H

#define PORT_IO_H

//#define _PORT_DWORD_OUT(x,y) __asm__("movl %0,%%eax\n movl %1, %%edx\n outl %%eax, %%dx"::"m"(x),"m"(y):"%eax","%edx") 
//#define _PORT_DWORD_IN(x,y) __asm__("movl %0,%%edx\n inl %%dx, %%eax\n movl %%eax,%1":"=m"(x):"m"(y):"%eax","%edx") 

#define _PORT_BYTE_OUT(x,y) __asm__("movl %0,%%eax\n movl %1, %%edx\n outb %%al, %%dx"::"g"(x),"g"(y):"%eax","%edx") 

#define _PORT_WORD_OUT(x,y) __asm__("movl %0,%%eax\n movl %1, %%edx\n outw %%ax, %%dx"::"g"(x),"g"(y):"%eax","%edx") 

#define _PORT_DWORD_OUT(x,y) __asm__("movl %0,%%eax\n movl %1, %%edx\n outl %%eax, %%dx"::"g"(x),"g"(y):"%eax","%edx") 

#define _PORT_BYTE_IN(x,y) __asm__("movl %1,%%edx\n xorl %%eax, %%eax\n inb %%dx, %%al\n movl %%eax,%0":"=m"(x):"g"(y):"memory","%eax","%edx") 

#define _PORT_WORD_IN(x,y) __asm__("movl %1,%%edx\n xorl %%eax, %%eax\n inw %%dx, %%ax\n movl %%eax,%0":"=m"(x):"g"(y):"memory","%eax","%edx") 

#define _PORT_DWORD_IN(x,y) __asm__("movl %1,%%edx\n inl %%dx, %%eax\n movl %%eax,%0":"=m"(x):"g"(y):"memory","%eax","%edx") 

#endif
