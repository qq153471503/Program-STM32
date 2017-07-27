#ifndef __EXIT_H
#define __EXIT_H	 
#include "sys.h"
 extern u16 temp;
#ifdef __EXIT_C__
#define GLOBAL
#else
#define GLOBAL extern
#endif

GLOBAL u8 	rxd1_timer4_1ms,rxd1_timer4_flag;



GLOBAL void Timer2_Init(u16 arr,u16 psc);	// 通用定时器中断初始化
GLOBAL void TIM2_IRQHandler(void);         // 定时中断2服务程序     

GLOBAL void Timer4_Init(u16 arr,u16 psc);	// 通用定时器中断初始化
GLOBAL void TIM4_IRQHandler(void);         // 定时中断4服务程序 	 				    

#undef GLOBAL
	 				    
#endif


