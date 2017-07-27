#include"delay.h"
#include"led.h"
#include"key.h"


int main(void)
{
	u8 flag ;
	Stm32ClockInit(9);
	DelayInit(72);
	LedInit();
	KeyInit();



	while(1)
	{
		
		flag = KeyScan();
		
			switch(flag)
			{
				case 1 : led0 = !led0; break;
				case 2 : led1 = !led1; break;
				case 3 : led2 = !led2; break;
				case 4 : led3 = !led3; break;
			}

			if(key0==0||key1==0||key2==0||key3==0)
			{
				beep = 0;
			}
			else 
				beep = 1;
	}
}



