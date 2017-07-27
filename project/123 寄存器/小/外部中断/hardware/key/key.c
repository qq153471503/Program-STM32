#include"key.h"
#include"delay.h"

void KeyInit(void)
{
	RCC -> APB2ENR |= 1<<2;
	GPIOA -> CRL &= 0XFFFFFFF0;
	GPIOA -> CRL |= 0X00000008;

	GPIOA -> CRH &= 0X0F0FFFFF;
	GPIOA -> CRH |= 0X80800000;
	GPIOA -> ODR |= 1<<13;
	GPIOA -> ODR |= 1<<15;
}  

u8 KeyScan(void)
{
	static u8 key_up = 1;

	JTAG_Set(JTAG_SWD_DISABLE);
	if(key_up&&(key0==0||key1==0||wk_up==1))
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
		}else if(wk_up == 1)
		{
			JTAG_Set(SWD_ENABLE);
			return 3;		
		}
	}
	else if(wk_up==0&&key0==1&&key1==1)
			key_up = 1;
		JTAG_Set(SWD_ENABLE);
		return 0;
}	
