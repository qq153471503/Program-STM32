#ifndef _MOTOR_H_
#define _MOTOR_H_

#include"sys.h"

#define LeftEn PEout(8)			  //EN1

//#define LeftOne PEout(9)		  //IN1
//#define LeftTwo PEout(11)		  //IN2
//#define RightOne PEout(12)		  //IN3
//#define RightTwo PEout(10)		  //IN4

#define LeftOne PEout(9)// PE9
#define RightOne PEout(10)// PE10
#define LeftTwo PEout(11)// PE11
#define RightTwo PEout(12)// PE12

#define RightEn PEout(13)		  //EN2

void MotorInit(void);

void Motor_Go(void);
void Motor_Back(void);
void Motor_Left(void);
void Motor_Right(void);
void Motor_Stop(void);

#endif
