#ifndef _KERNEL_H
#define _KERNEL_H

struct pciDevDescriptor
{
	uint32_t address;
	uint32_t class;
	uint32_t BAR[6];
	uint32_t space[6];
} *pciDevice;

#endif
