#ifndef _TIMER_H
#define _TIMER_H

#include"sys.h"

#define LeftOne PEout(9)// PE9
#define RightOne PEout(10)// PE10
#define LeftTwo PEout(11)// PE11
#define RightTwo PEout(12)// PE12

//#define LeftEn PEout(8)			  //EN1
//#define RightEn PEout(13)		  //EN2

#define LeftPwm TIM1 -> CCR1
#define RightPwm TIM1 -> CCR3

void PwmInit(u16 arr,u16 psc);
void Motor_Go(void);
void Motor_Back(void);
void Motor_Left(void);
void Motor_Right(void);
void Motor_Stop(void);


#endif

