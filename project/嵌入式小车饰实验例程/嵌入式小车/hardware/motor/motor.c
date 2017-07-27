#include"motor.h"

void MotorInit(void)
{
	RCC -> APB2ENR |= 1<<6 ;

	GPIOE -> CRH &= 0XFF000000;
	GPIOE -> CRH |= 0X00333333;

	GPIOE -> ODR &= 0x00; 
}
void Motor_Go(void)
{
	LeftOne = 0; 
	LeftTwo = 1; 
	RightOne = 0; 
	RightTwo = 1; 
}
void Motor_Back(void)
{
	LeftOne = 1; 
	LeftTwo = 0; 
	RightOne = 1; 
	RightTwo = 0; 
}
void Motor_Left(void)
{
	LeftOne = 1; 
	LeftTwo = 0;	
	RightOne = 0; 
	RightTwo = 1;	
}
void Motor_Right(void)
{
	LeftOne = 0; 
	LeftTwo = 1; 
	RightOne = 1; 
	RightTwo = 0;	
}
void Motor_Stop(void)
{
	LeftOne = 0; 
	LeftTwo = 0; 
	RightOne = 0; 
	RightTwo = 0; 
}

