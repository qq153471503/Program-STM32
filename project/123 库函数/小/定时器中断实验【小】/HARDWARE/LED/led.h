#ifndef _LED_H
#define _LED_H
#include"stm32f10x.h"

#define led0 PAout(8)	//╨Л╣ф
#define led1 PDout(2)	//бл╣ф

void LedInit(void);

#endif
