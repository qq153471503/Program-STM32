#include "ult.h"
#include "delay.h"

float dis;

void ULT_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);		//R  echo
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);		//T trlg
}

float ULT_Distance(u8 cnt)
{
	u8 i;
	u32 count = 0;
	float temp;
	
	ECHO = 0;
	TRLG = 0;
	
	for(i=0; i<cnt; i++)
	{
		TRLG = 1;
		delay_us(20);  /*ÖÁÉÙ10us*/
		TRLG = 0;
		while(!ECHO);
		while(ECHO)
		{
			delay_us(10);
			count++;
		}
		temp = temp+340/2*count*10;
		count = 0;
		delay_ms(20);
	}
	dis = temp/cnt/1000000+0.02;
	return dis;
}

