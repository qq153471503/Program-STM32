#include "key.h"
#include "led.h"
#include "delay.h"
#include "beep.h"
#include "exti.h"
#include "usart.h"
#include "sys.h"


int main(void)
{
	SystemInit();
	delay_init();
	NVIC_Configuration();
	BeepInit();
	LedInit();
	KeyInit();
	ExtiInit();
	uart_init(9600);
	
	while(1)
	{
		printf(" ϵͳ�����������У�OK \n\n");
		delay_ms(1800);
	}
}

