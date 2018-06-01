#include"../macro.h"

#ifndef _IO_H
#define _IO_H


/*
 * implementation of putchar function of C 
*/

void putchar(char c)
{
	if(c=='\n'){
		__asm__("movl $1,%eax");
		__asm__("int $0x80");
		__asm__("movl $2,%eax");
		__asm__("int $0x80");
	}
	else{
		__asm__("movl %0,%%edx"::"m"(c));
		__asm__("movl $0,%eax");
		__asm__("int $0x80");
	}
	
	return;
}

/*
 * implementation of puts function of C 
*/

void puts(char *str)
{
	unsigned int l;
	
	for(l=0;str[l];l++)
	{
		if(str[l]!='\n'){
			__asm__("movl %0,%%edx"::"m"(str[l]));
			__asm__("movl $0,%eax");
			__asm__("int $0x80");
		}
		else{
			__asm__("movl $1,%eax");
			__asm__("int $0x80");
			__asm__("movl $2,%eax");
			__asm__("int $0x80");
		}
	}
	
	return;
}


/*
 *partial implementation of printf function
*/

int printf(char *frmtstr,...)
{
	char c,number[31];
	void* ptr = &frmtstr;
	unsigned int count, l, l1, tmp, signflag;  
	
	number[30] = '\0';
	
	for(l=0;frmtstr[l];l++)
	{
		if(frmtstr[l]=='%'){
			l++;
			
			if(frmtstr[l]=='%'){
				putchar('%');
			}
			else{
				ptr=(char *)ptr+POINTER_WIDTH_BYTE;
				
				if(frmtstr[l]=='c'){
					putchar(*((char *)ptr));
					count++;
				}
				else if(frmtstr[l]=='s'){
					puts(*((char **)ptr));
				}
				else if(frmtstr[l]=='d' || frmtstr[l]=='x' || frmtstr[l]=='l' || frmtstr[l]=='o'){
					tmp = *((int *)ptr);
					signflag = 0;
					l1 = 30;

					if(tmp&0x80000000 && frmtstr[l]!='x'){
						signflag=1;
						tmp=~tmp+1;
					}

					if(frmtstr[l]=='d'){
						do{
							l1--;
							number[l1] = tmp%10+'0';
							tmp/=10;
						}while(tmp);
					}
					else if(frmtstr[l]=='x'){
						do{
							l1--;
							if((tmp&15)>=10){
								number[l1] = (tmp&15)+'a'-10;
							}
							else{
								number[l1] = (tmp&15)+'0';
							}
							tmp>>=4;
						}while(tmp);
					}
					else{
						do{
							l1--;
							number[l1] = (tmp&7)+'0';
							tmp>>=3;
						}while(tmp);
					}
					
					if(signflag){
						l1--;
						number[l1] = '-';
					}
					if(frmtstr[l] == 'x') {
						number[--l1] = 'x';
						number[--l1] = '0';
					}

					puts(&number[l1]);
					count+=30-l1;
				}
			} 
		}
		else{
			putchar(frmtstr[l]);
		}
	}
	
	return count;
}

/*
 *implementation of getchar function of C 
*/

unsigned int getchar()
{
	unsigned int data;
	
	__asm__("movl $0,%eax");
	__asm__("int $0x81");
	__asm__("movl %%eax,%0":"=m"(data):);
	
	putchar((char)data);
	
	return data;
}

#endif
