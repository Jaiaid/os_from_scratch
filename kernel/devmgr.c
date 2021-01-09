#include <stdint.h>
#include "devmgr.h"
#include "../portIO/port_IO.h"

extern void * _MEM_findalloc_page();

extern struct devDescriptor *device_list;
extern int numDevice;

void init_DEVMGR()
{
	device_list=(struct devDescriptor *)_MEM_findalloc_page();
}

int _DEVMGR_serve(int parameter, int arg)
{
	int service_no = parameter&0x0000ffff;
	int device_no = (parameter&0xffff0000)>>16;
	int ret;
	
	switch(service_no)
	{
		case 0:
			ret=attach((struct devDescriptor *)arg);
			break;
		case 1:
			ret=device_list[device_no].read((uint32_t *)arg);
			break;
		case 2:
			ret=device_list[device_no].write((uint32_t)arg);
			break;
		case 3:
			ret=close(device_no);
			break;
		default:
			ret=-1;
	};
	
	return ret;
}

int attach(struct devDescriptor *device)
{
	device_list[numDevice]=*device;
	device_list[numDevice].init();
	numDevice++;
	
	return numDevice-1;
}

int close(int device_no)
{
	device_list[device_no].close();
	numDevice--;
	return 0;
}

