#include"led.h"


void LedInit(void)
{
	RCC -> APB2ENR |= 1<<5;
	GPIOD -> CRH &= 0XFFFF0000;
	GPIOD -> CRH |= 0X00003333;
	GPIOD -> ODR |= 0X0F00;
}
