#include<stdint.h>

#include "../memory/memory.h"
#include "../lib/IO.h"
#include "../macro.h"
#include "../portIO/port_IO.h"
#include "devmgr.h"

extern struct IDT_DESC idt_descriptor;

extern void _ENABLE_A20();

extern void init_IDT();
extern void add_INT_Gate();
extern void add_TRP_Gate();

extern void init_DEVMGR(void);
extern void _DEVMGR_ENTRY(void);

extern void _TIMER_ENTRY(void);

extern void init_MEM(pagedir_entry *dir);
extern void *_MEM_findalloc_page();

extern int _DISPLAYDRIVER_init();
extern int _DISPLAYDRIVER_ENTRY1_printChar(uint32_t character);

int a20Enabled();
void enableA20();
void init_PIC(void);
void init_TIMER(void);
void double_fault();

struct devDescriptor *device_list;
int numDevice = 0;


void main()
{
	struct devDescriptor device;
	void *ptr = (void *)&device;
	/*checking if A20 line is enabled 
	 *if not then enabled
	 */
	if(!a20Enabled()) {
		// A20 disabled, enabling now
        _ENABLE_A20();
        while(!a20Enabled());
	}

	// initializing memory paging system
	// initializing paging, setting up kernel page table;
	init_MEM((pagedir_entry *)0xf000);

	// initilzing some known device like programmable interrupt controller
	// programmable interval timer
	// initializing 8259 PIC
	init_PIC();
	// initializing 8253/8254 programmable timer
	init_TIMER();
	// setting up interrupt table
	// setting up interrupt descriptor table
	init_IDT();

	// initializing device manager, allocating memory
	// this is only a list of i/o device and their init,read,write method kept by kernel
	// used as entry point for any device related service
	// see devmgr.c, devmgr.h
	init_DEVMGR();
	
	// setting up some interrupt services 
	// adding trap interrupt gate for accessing device manager service
	add_TRP_Gate(_DEVMGR_ENTRY, 0x80);
	// adding interrupt gate for scheduled preemption
	add_INT_Gate(_TIMER_ENTRY, 0x40);
	// adding interrupt gate for double fault exception\n
	add_INT_Gate(double_fault, 0x8);

	// setting up display output as device no. 0
	// no read method only init and write
	device.init=_DISPLAYDRIVER_init;
	device.write=_DISPLAYDRIVER_ENTRY1_printChar;
	device.read = 0;
	device.close = 0;

	// attaching the device by invoking interrupt service 
	__asm__("movl %0,%%edx"::"m"(ptr));
	__asm__("movl $0,%eax");
	__asm__("int $0x80");
	asm("sti");
	
	return;
}

void double_fault()
{
	puts("segmentation fault");
	__asm__("movl $0x45645,%eax");
	asm("hlt");
}

int a20Enabled()
{
	int32_t *ptr1=(int32_t *)0x1001fe;
	int32_t *ptr2=(int32_t *)0x1fe;

	*ptr1=0x12345678;

	if(*ptr1==*ptr2){
		*ptr1=0x1234;
		
		if(*ptr1==*ptr2){   //disabled
            return 0;
        }
	}

	return 1;
}

void init_PIC()
{
	_PORT_BYTE_OUT(PIC_RESET_CMD, MPIC_CMD_PORT);
	_PORT_BYTE_OUT(PIC_RESET_CMD, SPIC_CMD_PORT);
	_PORT_BYTE_OUT(MPIC_INT_OFFSET, MPIC_DATA_PORT);
	_PORT_BYTE_OUT(SPIC_INT_OFFSET, SPIC_DATA_PORT);
	_PORT_BYTE_OUT(SPIC_DATA_TO_MPIC, MPIC_DATA_PORT);
	_PORT_BYTE_OUT(MPIC_DATA_TO_SPIC, SPIC_DATA_PORT);
	_PORT_BYTE_OUT(ENV_PIC, MPIC_DATA_PORT);
	_PORT_BYTE_OUT(ENV_PIC, SPIC_DATA_PORT);
	_PORT_BYTE_OUT(MPIC_IMR, MPIC_DATA_PORT);
	_PORT_BYTE_OUT(SPIC_IMR, SPIC_DATA_PORT);
	
	return;
}

void init_TIMER()
{
	uint32_t timerData = TIMER_FREQUENCY/TIMER_INT_FREQUENCY; 
	
	_PORT_BYTE_OUT(TIMER_SETTING_CMD, TIMER_CMD_PORT);
	
	_PORT_BYTE_OUT(timerData, TIMER_DATA_PORT);
	timerData>>=8;
	_PORT_BYTE_OUT(timerData, TIMER_DATA_PORT);
	
	return;
}
