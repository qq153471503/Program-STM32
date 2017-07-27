#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"

#define wk_up PAin(0)
#define key2 PEin(2)
#define key1 PEin(3)
#define key0 PEin(4)

#define KEY_UP 		4
#define KEY_LEFT	3
#define KEY_DOWN	2
#define KEY_RIGHT	1


void KeyInit(void);
u8 KeyScan(u8 mode);

#endif

