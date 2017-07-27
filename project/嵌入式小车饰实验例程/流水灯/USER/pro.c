#include"led.h"
#include"delay.h"

int main(void)
{
	u32 i ;
	u32 k = 0x0800;
	u32 j = 0x0100;

		Stm32ClockInit(9);
		DelayInit(72);
		LedInit();

   while(1)
   {	
		for(i=0; i<4; i++)
		{
			GPIOD -> ODR |= k ;
			delayms(100);
			k >>= 1;
			GPIOD -> ODR = 0;
		}
		k = 0x0800 ;
	
		for(i=0; i<4; i++)
		{
			GPIOD -> ODR |= j ;
			delayms(100);
			j <<= 1;
			GPIOD -> ODR = 0;
		}
		j = 0x0100 ;
	}

}

