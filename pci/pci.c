#include<stdint.h>

#include"pci.h"
#include"../portIO/port_IO.h"

uint16_t _PCI_config_read_word(uint32_t bus,uint32_t slot,uint32_t func,uint32_t offset)
{
	uint32_t ioaddrs=0x80000000;
	uint32_t data;

	ioaddrs=ioaddrs|(bus<<16)|(slot<<11)|(func<<8)|(offset&0xfc);

	_PORT_DWORD_OUT(ioaddrs, PCI_CONFIG_ADDRS_REG);	
	_PORT_DWORD_IN(data, PCI_CONFIG_DATA_REG);

	data=(data>>((offset&2)*8))&0xffff;

	return (uint16_t)data;
}

uint32_t _PCI_read_register(uint32_t bus,uint32_t slot,uint32_t func,uint32_t registerno)
{
	uint32_t ioaddrs=0x80000000;
	uint32_t data;

	ioaddrs=ioaddrs|(bus<<16)|(slot<<11)|(func<<8)|(registerno&0xfc);

	_PORT_DWORD_OUT(ioaddrs, PCI_CONFIG_ADDRS_REG);	
	_PORT_DWORD_IN(data, PCI_CONFIG_DATA_REG);

	return data;
}

void _PCI_write_register(uint32_t bus,uint32_t slot,uint32_t func,uint32_t registerno, uint32_t data)
{
	uint32_t ioaddrs=0x80000000;

	ioaddrs=ioaddrs|(bus<<16)|(slot<<11)|(func<<8)|(registerno&0xfc);

	_PORT_DWORD_OUT(ioaddrs, PCI_CONFIG_ADDRS_REG);	
	_PORT_DWORD_OUT(data, PCI_CONFIG_DATA_REG);
}




