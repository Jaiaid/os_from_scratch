#ifndef _DEVMGR_H
#define _DEVMGR_H

#include <stdint.h>
#include "../portIO/port_IO.h"

struct devDescriptor
{
	int (*init)(void);
	int (*read)(uint32_t *var);
	int (*write)(uint32_t val);
	int (*close)();
};

int attach_device(struct devDescriptor *device);
int close_device(int device_no);

#endif
