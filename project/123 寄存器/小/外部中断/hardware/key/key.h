#ifndef _KEY_H_
#define _KEY_H_

#include"sys.h"
#include<stm32f10x_lib.h>

#define key0 PAin(13)
#define key1 PAin(15)
#define wk_up PAin(0)

void KeyInit(void);
u8 KeyScan(void);  

#endif
