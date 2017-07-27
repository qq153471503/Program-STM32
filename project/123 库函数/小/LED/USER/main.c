#include "sys.h"
#include "delay.h"
#include"led.h"

int main(void)
{
	delay_init(72);
	LedInit();

	while(1)
	{
		led0 = ~led0;
		delay_ms(500);
	}
}

