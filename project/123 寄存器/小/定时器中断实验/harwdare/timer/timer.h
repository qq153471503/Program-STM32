//#ifndef _TIMER_H
//#define _TIMER_H
//#include"sys.h"

//#define LedPWM TIM3 -> CCR2
//
//void PwmInit(u16 arr,u16 psc);
//void TimerInit(u16 arr,u16 psc);
//void timerscan(void);
//#endif

#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
//ͨ���ı�TIM3->CCR2��ֵ���ı�ռ�ձȣ��Ӷ�����LED0������

#define LED_PWM TIM3->CCR2 

void TimerInit(u16 arr,u16 psc);
void PWMInit(u16 arr,u16 psc);
#endif














































