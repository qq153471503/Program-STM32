#ifndef _KEY_H
#define _KEY_H

#include "stm32f10x.h"

#define key0  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)//读取按键0
#define key1  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)//读取按键1
#define key2  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)//读取按键2 
#define key3  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//读取按键3(WK_UP) 

void KeyInit(void);
void KeyScan(void);

extern char k0 ;
extern u8 count;
extern char i;
extern u8 table[];


#endif

