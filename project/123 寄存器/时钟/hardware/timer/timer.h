#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

#define enable {TIM3->DIER|=1<<0;  	TIM3->DIER|=1<<6;   TIM3->CR1|=0x01;}   			//�򿪶�ʱ��
#define disable	{TIM3->DIER &= 0<<0;  	TIM3->DIER &= 0<<6;   TIM3->CR1 &= 0x00; }  			//�رն�ʱ��

void TimerInit(u16 arr,u16 psc);
#endif
