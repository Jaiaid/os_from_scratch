#include<stdint.h>
#include"../macro.h"
#include"../portIO/port_IO.h"

#define VIDEO_MEMORY_OFFSET 0xb8000 
#define MAX_CHAR_IN_SCREEN 2000
#define MAX_CHAR_IN_ROW 80
#define MAX_ROW 80
#define SCREEN_COLOR 0x07
#define SCRN_CTRL_REG  0x3d4
#define SCRN_DATA_REG  0x3d5
#define HIGH_BYTE_SELECTOR  0x0e
#define LOW_BYTE_SELECTOR  0x0f


uint16_t *video_memory = (uint16_t *)(VIDEO_MEMORY_OFFSET-CODE_SEG_PHY_ADRS);
uint32_t abspos = 1;
uint32_t xpos = 1;


int _DISPLAYDRIVER_init(void)
{
	unsigned int l;
	abspos=0;
	xpos=0;
	for(l=0;l<2000;l++) video_memory[l] = (SCREEN_COLOR<<8) + 0;

	return 0;
}


void cursorUpdate()
{
	_PORT_BYTE_OUT(HIGH_BYTE_SELECTOR,SCRN_CTRL_REG);
	_PORT_BYTE_OUT((abspos>>8),SCRN_DATA_REG);
	_PORT_BYTE_OUT(LOW_BYTE_SELECTOR,SCRN_CTRL_REG);
	_PORT_BYTE_OUT(abspos,SCRN_DATA_REG);
	
	return;
}


int _DISPLAYDRIVER_ENTRY1_printChar(uint32_t character)
{
	video_memory[abspos] = (SCREEN_COLOR<<8) + (char)character; 
	abspos++;
	xpos++;
	
	if(xpos>MAX_CHAR_IN_ROW){
		xpos = 0;
	}
	
	cursorUpdate();
	
	return abspos; 
}


int _DISPLAYDRIVER_ENTRY2_nextLine(void)
{
	abspos+=MAX_CHAR_IN_ROW;
	cursorUpdate();
	
	return abspos;
}


int _DISPLAYDRIVER_ENTRY3_lineFirst(void)
{
	abspos-=xpos;
	xpos=0;
	cursorUpdate();
	
	return abspos;
}

