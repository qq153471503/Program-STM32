#include"key.h"
#include"delay.h"


void KeyInit(void)
{
	RCC -> APB2ENR |= 1<<3 ;

	GPIOC -> CRH &= 0X0000FFFF;
	GPIOC -> CRH |= 0X88880000;

	GPIOC -> ODR |= 0XF000;
}

u8 KeyScan(void)
{
	static u8 key_up = 1;
	JTAG_Set(JTAG_SWD_DISABLE);
	if(key_up&&(key0==0||key1==0||key2==0||key3==0))
	{
		delayms(10);

		key_up = 0;

		if(key0 == 0)
		{
			JTAG_Set(SWD_ENABLE);
			return 1;
		}
		else if(key1 == 0)
		{
			JTAG_Set(SWD_ENABLE);
			return 2;
		}
		else if(key2 == 0)
		{
			JTAG_Set(SWD_ENABLE);
			return 3;
		}
		else if(key3 == 0)
		{
			JTAG_Set(SWD_ENABLE);
			return 4;
		}
	}
	else if(key0==1&&key1==1&&key2==1&&key3==1)
		  key_up = 1;
		  JTAG_Set(SWD_ENABLE);
		return 0 ;

}
