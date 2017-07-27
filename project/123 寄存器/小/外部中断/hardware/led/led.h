#ifndef _LED_H_
#define _LED_H_

#include<stm32f10x_lib.h>
#include"sys.h"

#define led0 PAout(8)
#define led1 PDout(2)

void LedInit(void);

#endif
