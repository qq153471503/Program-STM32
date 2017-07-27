#include"led.h"
#include"pwm.h"
#include"delay.h"

int main(void)
{
	u8 flag = 1 ;
	u16 value = 0;
	SystemInit();
	delay_init(72);
	LedInit();
	PwmInit(900,0);

	while(1)
	{
		delay_ms(5);
		if(flag)
			value++;
		else
			value--;

		if(value > 300)	flag = 0;
		if(value == 0)	flag = 1;

		TIM_SetCompare2(TIM3,value);	   
	}
}


