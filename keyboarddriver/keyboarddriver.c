#include<stdint.h>
#include"../portIO/port_IO.h"

#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_DATA_PORT 0x60

extern char _SCS1_TO_ASCII_MAP[256];

uint32_t _KEYBOARDDRIVER_ENTRY1_READCHAR(void)
{
	uint32_t scanCode,val,dummy;
	
	while((_PORT_BYTE_IN(val,KEYBOARD_STATUS_PORT) & 1) == 0);
	
	_PORT_BYTE_IN(scanCode,KEYBOARD_DATA_PORT);
	val = _SCS1_TO_ASCII_MAP[scanCode];
	
	while((_PORT_BYTE_IN(KEYBOARD_STATUS_PORT) & 1) == 0);
	_PORT_BYTE_IN(dummy,KEYBOARD_DATA_PORT);
	
	return val;
}
