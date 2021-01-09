#include<stdint.h>
#include"memory.h"

void init_MEM();
void _MEM_set_pagedir_entry(pagedir_entry *dir, uint32_t entryno, uint32_t addrs);
void *_MEM_findalloc_page();
void _MEM_set_page(pagedir_entry *dirptr,uint32_t virtual_page_no);

extern void _MEM_enable_paging();
extern void _MEM_set_pagedirectory(void *pagedirAdddrs);
/*
 *32MiB has 2^13 4KiB page. Each bit of an 4 byte element represents a page. So
 *MEMPAGE/(8*4) element
*/
uint32_t memmap[MEMPAGE/(8*4)]={0xff};


void init_MEM(pagedir_entry *dir)
{
	uint32_t virtual_page_no,ptr;
	pagetable_entry *table;
	memmap[0]=0x00001fff;
	memmap[5]=0x00002000;

	_MEM_set_pagedirectory((unsigned char*)dir);
	_MEM_set_pagedir_entry(dir,0,0x10000);

	table = (pagetable_entry *)(((uint32_t)dir[0].addrs) & 0xfffff000);
	table[7].content=(uint32_t *)0x00007001;
    table[8].content=(uint32_t *)0x00008001;
    table[9].content=(uint32_t *)0x00009001;
	for(virtual_page_no=0xa,ptr=0xa000;virtual_page_no<20;virtual_page_no++,ptr+=0x1000)
	{
	    table[virtual_page_no].content=(uint32_t *)(ptr|0x00000003);
	}
	table[0xb8].content=(uint32_t *)(0xb8003);
	table[0xb9].content=(uint32_t *)(0xb9003);
    table[0x7f].content=(uint32_t *)(0x7f003);

	_MEM_enable_paging();
}

void _MEM_set_pagedir_entry(pagedir_entry *dir, uint32_t entryno, uint32_t addrs)
{
	dir[entryno].addrs = (pagetable_entry *)(addrs|0x00000003);
}

void _MEM_set_page(pagedir_entry *dir,uint32_t virtual_page_no)
{
	void *ptr=_MEM_findalloc_page();

	if(ptr==0) return;

	dir[virtual_page_no>>20].addrs[(virtual_page_no>>12)&0xfff].content=ptr;
}

void *_MEM_findalloc_page()
{
	int l,l1;

	for(l=0;l<MEMPAGE/4;l++)
	{
		for(l1=0;l1<32;l1++)
		{
			if((memmap[l]&(1<<l1))==0){
				memmap[l]|=(1<<l1);     //mark the page as allocated(=1)
				return (void *)(((l<<5)+l1)<<12); //return base address of allocated page;
			}
		}
	}

	return 0;
}
