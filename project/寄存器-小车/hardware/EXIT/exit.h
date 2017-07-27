#ifndef __EXIT_H
#define __EXIT_H	 
#include "sys.h"


void Timer2_Init(u16 arr,u16 psc);	// 通用定时器中断初始化
void TIM2_IRQHandler(void);         // 定时中断3服务程序     

	 				    
#endif


