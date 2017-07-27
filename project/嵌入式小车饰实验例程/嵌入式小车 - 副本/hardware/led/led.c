#include"led.h"


void LedInit(void)
{
	RCC -> APB2ENR |= 1<<5;
	GPIOD -> CRH &= 0XFFF00000;
	GPIOD -> CRH |= 0X00033333;
	GPIOD -> ODR |= 0X1F00;
}
