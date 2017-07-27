#include"led.h"
#include"timer.h"

void PWMInit(u16 arr,u16 psc)
{
	RCC -> APB1ENR |= 1<<1;

	GPIOA -> CRH &= 0XFFFFFFF0;
	GPIOA -> CRH |= 0X00000004;

	GPIOA -> CRL &= 0X0FFFFFFF;
	GPIOA -> CRL |= 0XB0000000;
	GPIOA -> ODR |= 1<<7;

	TIM3 -> ARR = arr;
	TIM3 -> PSC = psc;
	
	TIM3 -> CCMR1 |= 7<<12;
	TIM3 -> CCMR1 |= 1<<11;

	TIM3 -> CCER |= 1<<4;

	TIM3 -> CR1 = 0X8000;
	TIM3 -> CR1 |= 0X01;
}

//void TimerInit(u16 arr,u16 psc)
//{
//	RCC -> APB1ENR |= 1<<1;	
//	TIM3 -> ARR = arr ;
//	TIM3 -> PSC = psc ;
//	TIM3 -> DIER |= 1<<0;
//	TIM3 -> DIER |= 1<<6;
//	TIM3 -> CR1 |= 0X01;
//	MY_NVIC_Init(1,3,TIM3_IRQChannel,2);
//}  	 













