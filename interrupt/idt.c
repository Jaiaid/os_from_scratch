#include<stdint.h>
#include"../macro.h"

extern void *_MEM_findalloc_page();

struct IDT_DESC
{
	uint16_t size;
    uint16_t baseLinAddrsLowWord;
    uint16_t baseLinAddrsHighWord;
    uint16_t pad;
}
idt_descriptor = {1,1};

struct IDT_ENTRY
{
	uint16_t ofstprt1;
	uint16_t selector;
	uint16_t type;
	uint16_t ofstprt2;
} *idt;


void init_IDT()
{
	idt = (struct IDT_ENTRY *)_MEM_findalloc_page();
	idt_descriptor.baseLinAddrsLowWord = ((uint32_t)idt + CODE_SEG_PHY_ADRS)&0xffff;
    idt_descriptor.baseLinAddrsHighWord = ((uint32_t)idt + CODE_SEG_PHY_ADRS)>>16;	
    idt_descriptor.size = 2047;
	asm volatile("lidt %0"::"m"(idt_descriptor));
}

void add_INT_Gate(void *fptr(),unsigned int int_num)
{
	idt[int_num].ofstprt1 = (uint32_t)fptr;
	idt[int_num].selector = CODE_SEG_SELECTOR;
	idt[int_num].type = 0x8E00;
	idt[int_num].ofstprt2 = ((uint32_t)fptr>>16);
}

void add_TRP_Gate(void *fptr(),unsigned int int_num)
{
	idt[int_num].ofstprt1 = (uint32_t)fptr;
	idt[int_num].selector = CODE_SEG_SELECTOR;
	idt[int_num].type = 0x8F00;
	idt[int_num].ofstprt2 = ((uint32_t)fptr>>16);
}
