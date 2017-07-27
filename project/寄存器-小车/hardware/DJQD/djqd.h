#ifndef __DJQD_H
#define __DJQD_H	 
#include "sys.h"
#include "delay.h"

//电机驱动端口定义

//#define LEFT_PWM PEout(8)  // PE8
#define LEFT111 PEout(9)  // PE9
#define RIGHT112 PEout(10) // PE10
#define LEFT112 PEout(11) // PE11
#define RIGHT111 PEout(12)  // PE12
//#define RIGHT_PWM PEout(13)  // PE13
//#define LEFT_PWM_Value  TIM1->CCR1 
//#define RIGHT_PWM_Value TIM1->CCR3 

//#define STOP  LEFT_PWM_Value=60000;RIGHT_PWM_Value=60000;LEFT111=0;LEFT112=0;RIGHT112=0;RIGHT111=0;//停车
	
void DJ_Init(void);
void PWM_Init(void);
void STOP (void);
void Control(int L_Spend,int R_Spend);
		 				    
#endif

