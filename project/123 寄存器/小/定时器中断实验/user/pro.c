#include "delay.h"	
#include "led.h" 
#include "timer.h"	 	 


int main(void)
{
	u16 pwmvalue = 0;
	u8 dir = 1;

	Stm32ClockInit(9);
	DelayInit(72);
 	LedInit();
	PWMInit(900,0);


	while(1)
	{
		delayms(10);

		if(dir) pwmvalue++;
		else pwmvalue--;

		if(pwmvalue>100) 	dir = 0;
		if(pwmvalue == 0)	dir = 1;

		LED_PWM = pwmvalue ;
	}
}





















































