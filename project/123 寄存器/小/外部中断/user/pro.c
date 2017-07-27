#include"key.h"
#include"led.h"
#include"delay.h"
#include"usart.h"
#include"exti.h"

int main(void)
{
	Stm32ClockInit(9);
	DelayInit(72);
	uart_init(72,9600);
	LedInit();
	ExtiInit();

	while(1)
	{
		printf("OK\n");
		delayms(1000);
	}
}


