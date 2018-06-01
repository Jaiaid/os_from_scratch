#ifndef _MACRO_H

#define _MACRO_H

#define POINTER_WIDTH_BYTE 4

#define CODE_SEG_PHY_ADRS 0x0000 
#define DATA_SEG_PHY_ADRS 0x0000 
#define CODE_SEG_SELECTOR 0x08

#define PIC_RESET_CMD 0x11
#define MPIC_CMD_PORT 0x20
#define MPIC_DATA_PORT 0x21
#define SPIC_CMD_PORT 0xa0
#define SPIC_DATA_PORT 0xa1
#define MPIC_IMR 0xfe
#define SPIC_IMR 0xff
#define MPIC_INT_OFFSET 0x40
#define SPIC_INT_OFFSET 0x48
#define SPIC_DATA_TO_MPIC 0x04
#define MPIC_DATA_TO_SPIC 0x02
#define ENV_PIC 0x01

#define TIMER_CMD_PORT 0x43
#define TIMER_DATA_PORT 0x40
#define TIMER_SETTING_CMD 0x36
#define TIMER_FREQUENCY 1193182
#define TIMER_INT_FREQUENCY 20

#endif
