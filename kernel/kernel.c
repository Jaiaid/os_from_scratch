#include<stdint.h>

#include"../memory/memory.h"
#include"../lib/IO.h"
#include"../macro.h"
#include"../portIO/port_IO.h"

extern struct IDT_DESC idt_descriptor;

extern void _ENABLE_A20();

extern void init_IDT();
extern void add_INT_Gate();
extern void add_TRP_Gate();

extern void _DISPLAYDRIVER_init(void);
extern void _DISPLAYDRIVER_ENTRY(void);

extern void _TIMER_ENTRY(void);

extern uint16_t _PCI_config_read_word(uint32_t bus,uint32_t slot,uint32_t func,uint32_t offset);
extern uint32_t _PCI_read_register(uint32_t bus,uint32_t slot,uint32_t func,uint32_t offset);
extern void _PCI_write_register(uint32_t bus,uint32_t slot,uint32_t func,uint32_t offset,uint32_t data);

extern void _MEM_init_kernel(pagedir_entry *dir);
extern void *_MEM_findalloc_page();

int a20Enabled();
void enableA20();
void _PIC_init(void);
void _TIMER_init(void);
void double_fault();

struct pciDevDescriptor
{
	uint32_t address;
	uint32_t class;
	uint32_t BAR[6];
	uint32_t space[6];
} *pciDevice;

struct devDriverDescriptor
{
	void (*read)();
	void (*write)();
} *device;

uint32_t pciDevCount = 0;
int ideDeviceIndx = 0;
int sataDeviceIndx = 0;

uint32_t pciDevDiscover(uint32_t initBus, uint32_t count, struct pciDevDescriptor *pciDevList);
int findPCIDevice(uint32_t classCode, struct pciDevDescriptor *pciDevList, int count);

void main()
{
	int l;
	/*checking if A20 line is enabled 
	 *if not then enabled
	 */
	if(!a20Enabled()) {
        	_ENABLE_A20();
        	while(!a20Enabled());
	}

	_MEM_init_kernel((pagedir_entry *)0xf000);
	_PIC_init();
	_TIMER_init();

	_DISPLAYDRIVER_init();
	init_IDT();

	add_TRP_Gate(_DISPLAYDRIVER_ENTRY, 0x80);
	add_INT_Gate(_TIMER_ENTRY, 0x40);
	add_INT_Gate(double_fault, 0x8);

	asm("sti");

	pciDevice = _MEM_findalloc_page();
	pciDevCount = pciDevDiscover(0, 0, pciDevice);
	printf("Total %d pci device found\n", pciDevCount);

	ideDeviceIndx = findPCIDevice(0x101, pciDevice, pciDevCount);
	sataDeviceIndx = findPCIDevice(0x106, pciDevice, pciDevCount); 
	if(ideDeviceIndx != -1) {
		printf("IDE storage found at pci address %x\n", pciDevice[ideDeviceIndx].address);
		for(l = 0;l < 6;l++)
		{
			printf("BAR%d = %x, size = %x\n", l, pciDevice[ideDeviceIndx].BAR[l], pciDevice[ideDeviceIndx].space[l]);
		}
	}
	else {
		printf("IDE storage not found\n"); 
	}

	if(sataDeviceIndx != -1) {
		printf("\nSATA storage found at pci address %x\n", pciDevice[sataDeviceIndx].address);
		for(l = 0;l < 6;l++)
		{
			printf("BAR%d = %x, size = %x\n", l, pciDevice[sataDeviceIndx].BAR[l], pciDevice[sataDeviceIndx].space[l]);
		}
	}
	else {
		printf("\nSATA storage not found\n"); 
	}

	return;
}

void double_fault()
{
	puts("segmentation fault");
	asm("hlt");
}

int a20Enabled()
{
	int32_t *ptr1=0x1001fe;
	int32_t *ptr2=0x1fe;

	*ptr1=0x12345678;

	if(*ptr1==*ptr2){
		*ptr1=0x1234;
		
		if(*ptr1==*ptr2){   //disabled
            		return 0;
        	}
	}

	return 1;
}

void _PIC_init()
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

void _TIMER_init()
{
	uint32_t timerData = TIMER_FREQUENCY/TIMER_INT_FREQUENCY; 
	
	_PORT_BYTE_OUT(TIMER_SETTING_CMD, TIMER_CMD_PORT);
	
	_PORT_BYTE_OUT(timerData, TIMER_DATA_PORT);
	timerData>>=8;
	_PORT_BYTE_OUT(timerData, TIMER_DATA_PORT);
	
	return;
}

uint32_t pciDevDiscover(uint32_t initBus, uint32_t count, struct pciDevDescriptor *pciDevList)
{
	uint32_t vendor, class, devid, header, downstreamBus;
	uint32_t address, neededMem;
	uint32_t slot, funcNo, BARNo, totalFunc;

	for(slot=0;slot<32;slot++)
	{

		vendor=_PCI_config_read_word(initBus,slot,0,0);

		if(vendor!=0xffff){
			header=_PCI_config_read_word(initBus,slot,0,14);

			if(header&0x80) totalFunc=8;
			else totalFunc=1;

			for(funcNo=0;funcNo<totalFunc;funcNo++)
			{
				vendor=_PCI_config_read_word(initBus,slot,funcNo,0);

				if(vendor==0xffff) continue;

				header = _PCI_config_read_word(initBus,slot,funcNo,14);
				devid = _PCI_config_read_word(initBus,slot,funcNo,2);
				class = _PCI_config_read_word(initBus,slot,funcNo,10);

				address = _PCI_read_register(initBus,slot,funcNo,20);
				_PCI_write_register(initBus, slot, funcNo, 20, 0xffffffff);
				neededMem = _PCI_read_register(initBus,slot,funcNo,20);

				if(class == 0x0604) {
					downstreamBus = (_PCI_config_read_word(initBus,slot,funcNo,24)>>8);
					count = pciDevDiscover(downstreamBus, count+1, pciDevList);
				}

				//printf("PCI device : bus = %x, slot = %x, devfn = %x\n",initBus, slot, funcNo);
				//printf("device id = %x, class = %x, vendor = %x, header = %x\n", devid, class, vendor, header);

				for(BARNo = 0;BARNo < 6;BARNo++)
				{
					address = _PCI_read_register(initBus,slot,funcNo, 20+(BARNo<<2));
					_PCI_write_register(initBus, slot, funcNo, 20+(BARNo<<2), 0xffffffff);
					neededMem = _PCI_read_register(initBus, slot, funcNo, 20+(BARNo<<2));

					if(address&1) neededMem = (neededMem&0xfffffffc);
					else neededMem = (neededMem&0xfffffff0);

					neededMem = (~neededMem) + 1;

					//printf("BAR%d address = %x, size = %x\n", BARNo, address, neededMem);
					pciDevList[count].BAR[BARNo] = address;
					pciDevList[count].space[BARNo] = neededMem;

					_PCI_write_register(initBus, slot, funcNo, 20+(BARNo<<2), address);
				}

				pciDevList[count].class = class;
				pciDevList[count].address = (0x80000000)|(initBus<<16)|(slot<<11)|(funcNo<<8);

				count++;
			}
		}
	}

	return count;
}

int findPCIDevice(uint32_t classCode, struct pciDevDescriptor *pciDevList, int count)
{
	int l;
	for(l = 0;l < count;l++)
	{
		if(pciDevList[l].class == classCode) return l;
	}

	return -1;
}
