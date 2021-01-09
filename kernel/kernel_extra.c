uint32_t pciDevCount = 0;
int ideDeviceIndx = 0;
int sataDeviceIndx = 0;

uint32_t pciDevDiscover(uint32_t initBus, uint32_t count, struct pciDevDescriptor *pciDevList);
int findPCIDevice(uint32_t classCode, struct pciDevDescriptor *pciDevList, int count);

extern uint16_t _PCI_config_read_word(uint32_t bus,uint32_t slot,uint32_t func,uint32_t offset);
extern uint32_t _PCI_read_register(uint32_t bus,uint32_t slot,uint32_t func,uint32_t offset);
extern void _PCI_write_register(uint32_t bus,uint32_t slot,uint32_t func,uint32_t offset,uint32_t data);

{
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

