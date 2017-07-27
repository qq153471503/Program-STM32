#include"beep.h"

void BeepInit(void)
{
	RCC -> APB2ENR |= 1<<5;
	GPIOD -> CRH &= 0XFFF0FFFF;
	GPIOD -> CRH |= 0X00030000;
	GPIOD -> ODR |= 1<<12;
}
