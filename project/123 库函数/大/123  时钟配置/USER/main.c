#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "system.h"

int main(void)
{
	RCC_Config();
	delay_init();
	uart_init(115200);
	LED_Init();
	
	while(1)
	{
		LED0 = !LED0;
		delay_ms(500);
	}
}





