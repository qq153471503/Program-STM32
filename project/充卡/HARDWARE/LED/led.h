#ifndef __LED_H
#define __LED_H	 
#include "sys.h"


//LED端口定义
#define LED0 PDout(8)// PD8
#define LED1 PDout(9)// PD9
#define LED2 PDout(10)// PD10
#define LED3 PDout(11)// PD11


	

void LED_Init(void);//初始化		 				    
#endif

















