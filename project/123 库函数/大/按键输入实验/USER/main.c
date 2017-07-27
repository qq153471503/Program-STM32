#include "key.h"
#include "led.h"
#include "delay.h"
#include "beep.h"


int main(void)
{
	u8 t ;
	
	SystemInit();
	delay_init();
	BeepInit();
	LedInit();
	KeyInit();
	
	while(1)
	{
		t = KeyScan(0);
		
		if(t)
		{
			switch(t)
			{
				case 1: led0 = !led0;	break;
				case 2: led1 = !led1;	break;
				case 3: led0 = !led0;
						led1 = !led1;	break;
				case 4: beep = !beep;	break;
			}
		}
		else delay_ms(10);
	}
}

