#include"exti.h"
#include"led.h"
#include"usart.h"
#include"key.h"
#include"delay.h"

void EXTI15_10_IRQHandler(void)
{
	delayms(10);
	if( key0 == 0 )
	{
		led0 = !led0;
	}
	else if(key1 == 0)
	{
		led1 = !led1;
	}

	EXTI -> PR = 1<<13;
	EXTI -> PR = 1<<15;
}
void EXTI0_IRQHandler(void)
{
	delayms(10);

	if(wk_up == 1)
	{
		led0 = !led0;
		led1 = !led1;
	}

	EXTI -> PR = 1<<0;
}

void ExtiInit(void)
{
	RCC -> APB2ENR |= 1<<2;
	JTAG_Set(JTAG_SWD_DISABLE);
	GPIOA -> CRL &= 0XFFFFFFF0;
	GPIOA -> CRL |= 0X00000008;
	GPIOA -> CRH &= 0X0F0FFFFF;
	GPIOA -> CRH |= 0X80800000;
	GPIOA -> ODR |= 1<<13;
	GPIOA -> ODR |= 1<<15;

	Ex_NVIC_Config(GPIO_A,0,RTIR);
	Ex_NVIC_Config(GPIO_A,13,FTIR);
	Ex_NVIC_Config(GPIO_A,15,FTIR);

	MY_NVIC_Init(2,2,EXTI0_IRQChannel,2);
	MY_NVIC_Init(2,1,EXTI15_10_IRQChannel,2);
}





