#ifndef __DJQD_H
#define __DJQD_H	 
#include "sys.h"
#include "delay.h"

//电机驱动端口定义

#define LEFT_NSLEEP  PEout(8)     // PE8
#define REFT_NSLEEP  PEout(10)    // PE10

#define REFT1_PWM    PEout(13)    // PE13
#define REFT2_PWM    PEout(14)    // PE14

#define LEFT1_PWM    PEout(9)     // PE9
#define LEFT2_PWM    PEout(11)    // PE11

void DJ_Init(void);
void PWM_Init(void);
void STOP (void);
void Control(int L_Spend,int R_Spend);
void Go_Test( u8 sp ,u8 len) ;
void Back_Test( u8 sp ,u8 len);
void Left_Test( u8 sp);
void Right_Test(u8 sp);
void Track_Test( u8 sp);
void Stop_Test(void);
		 				    
#endif

