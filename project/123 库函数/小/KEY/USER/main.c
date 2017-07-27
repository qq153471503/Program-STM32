
	#include"key.h"
	#include"led.h"
	#include"delay.h"

int main(void)
{
	u8 t ;
	SystemInit();
	delay_init(72);
	LedInit();
	KeyInit();	
	while(1)
	{
		t = KeyScan(0);

			switch(t)
			{
				case 1: 
						led0 = !led0;	
						break;
				case 2: 
						led1 = !led1;
						break;
				case 3: led1 = !led1;
						led0 = !led0;
						break;
			}
	}	
}
