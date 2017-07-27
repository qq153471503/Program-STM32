#ifndef __CSB_H
#define __CSB_H	 
#include "sys.h"

#define CSB_TX PEout(0)             // PE0发送超声波引脚

#define INT0      PBin(9)				//定义超声波中断输入引脚
#define SPEED     PBin(10)			    //定义码盘信号输入引脚
#define SPEED_R   PBin(0)			    //定义码盘信号输入引脚

extern u16 dis;

void EXTI9_5_IRQHandler(void);    // 外部中断13服务程序
void EXTIX_Init(void);		        // 外部中断初始化程序
void Timer3_Init(u16 arr,u16 psc);	// 通用定时器中断初始化
void TIM3_IRQHandler(void);         // 定时中断3服务程序     
void tran(void); 					// 左发生超声波函数
	 				    
#endif


