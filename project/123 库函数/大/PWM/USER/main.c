#include "stm32f10x.h"
#include "led.h"
#include "pwm.h"
#include "delay.h"

u16 value = 0;

int main(void)
{
	u8 flag = 1;
	
	SystemInit();
	delay_init();
	LedInit();
	
	PwmInit(899,0);
	
	while(1)
	{
		delay_ms(10);
		
		if(flag)
			value++;
		else 
			value--;
		
		if(value == 300)
			flag = 0;
		if(value == 0)
			flag = 1;
			
		
		TIM_SetCompare2(TIM3,value);
	}
}

