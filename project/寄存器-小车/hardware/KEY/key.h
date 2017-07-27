#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"

//按键端口定义
#define KEY0 PBin(12)   //PB12
#define KEY1 PBin(13)	//PB13 
#define KEY2 PBin(14)	//PB14
#define KEY3 PBin(15)   //PB15
	 
void KEY_Init(void);//IO初始化
				    
#endif
