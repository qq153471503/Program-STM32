#include "timer.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "beep.h"

int main(void)
{
	u8 count = 0;
	
	SystemInit();
	delay_init();
	uart_init(9600);
	LedInit();
	BeepInit();
	TPAD_Init(6);
	
	while(1)
	{
		if(TPAD_Scan(1))
		{
			led1 = !led1;
			beep = 1;
		}
		else
		{
			beep = 0;
		}
		
		count++;
		
		if(count == 10)
		{
			count = 0;
			led0 = !led0;/*red right*/
		}
		
		delay_ms(10);
	}
	
}



