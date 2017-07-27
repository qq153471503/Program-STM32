#include "key.h"
#include "delay.h"

void KeyInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}
u8 KeyScan(u8 mode)
{
	static u8 key_up = 1;
	
	if(mode)	key_up = 1;
	
	if(key_up&&(key0 == 0||key1 == 0||key2 == 0||wk_up == 1))
	{
		delay_ms(10);
		key_up = 0;
		
		if(key0 == 0)
			return 1;
		else if(key1 == 0)
			return 2;
		else if(key2 == 0)
			return 3;
		else if(wk_up == 1)
			return 4;
	}else if(key0 == 1&&key1 == 1&&key2 == 1&&wk_up == 0)
		key_up = 1;
		return 0;
}

