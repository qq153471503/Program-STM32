#include"timer.h"

void PwmInit(u16 arr,u16 psc)
{
	RCC -> APB2ENR |= 1<<11;
	RCC -> APB2ENR |= 1<<6;
	GPIOE -> CRH &= 0XFF000000;
	GPIOE -> CRH |= 0X00B3333B;

	RCC -> APB2ENR |= 1<<0;
	AFIO -> MAPR &= 0XFFFFFF3F;
	AFIO -> MAPR |= 1<<7;
	AFIO -> MAPR |= 1<<6;

	TIM1 -> ARR = arr;
	TIM1 -> PSC = psc;

	TIM1 -> CCMR2 |= 6<<4;
	TIM1 -> CCMR2 |= 1<<3;

	TIM1 -> CCMR1 |= 6<<4;
	TIM1 -> CCMR1 |= 1<<3;

	TIM1 -> CR1 |= 1<<7;
	TIM1 -> CR1 |= 1<<4;

	TIM1 -> CCER |= 3<<2;
	TIM1 -> CCER |= 3<<8;

	TIM1 -> BDTR |= 1<<15;

	TIM1 -> EGR |= 1<<0;
	TIM1 -> CR1 |= 1<<0;
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

