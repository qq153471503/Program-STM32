#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

#define enable {TIM3->DIER|=1<<0;  	TIM3->DIER|=1<<6;   TIM3->CR1|=0x01;}   			//打开定时器
#define disable	{TIM3->DIER &= 0<<0;  	TIM3->DIER &= 0<<6;   TIM3->CR1 &= 0x00; }  			//关闭定时器

void TimerInit(u16 arr,u16 psc);
#endif
