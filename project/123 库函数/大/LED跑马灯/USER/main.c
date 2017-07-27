#include "led.h"
#include "delay.h"
#include "system.h"

int main(void)
{
	RCC_Config();
	delay_init();
	
	LedInit();
	
	while(1)
	{
		led0 = 1;
		led1 = 0;
		delay_ms(300);
		led0 = 0;
		led1 = 1;
		delay_ms(300);
	}
}

