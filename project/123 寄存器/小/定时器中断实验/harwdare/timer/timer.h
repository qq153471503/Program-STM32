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
//通过改变TIM3->CCR2的值来改变占空比，从而控制LED0的亮度

#define LED_PWM TIM3->CCR2 

void TimerInit(u16 arr,u16 psc);
void PWMInit(u16 arr,u16 psc);
#endif














































