#include "timer.h"
#include "usart.h"
#include "delay.h"

#define TPAD_ARR_MAX_VAL 0XFFFF
#define TPAD_GATE_VAL 100
vu16 tpad_default_val = 0;


void TIM5_CH2_Cap_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	TIM_InitStructure.TIM_Period = arr;
	TIM_InitStructure.TIM_Prescaler = psc;
	TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM5,&TIM_InitStructure);
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICPrescaler = 0;
	TIM_ICInitStructure.TIM_ICFilter = 0X03;  //采集八次 为一次输出
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM5,&TIM_ICInitStructure);
	
	TIM_Cmd(TIM5,ENABLE);	
}
u8 TPAD_Init(u8 psc)
{
	u16 buf[10];
	u16 temp;
	u8 i,k;
	TIM5_CH2_Cap_Init(TPAD_ARR_MAX_VAL,psc-1);
	for(i=0; i<10; i++)
	{
		buf[i] = TPAD_Get_Val();
		delay_ms(10);
	}
	
	for(i=0; i<9; i++)
	{
		for(k=i+1; k<10; k++)
		{
			if(buf[i]>buf[k])
			{
				temp = buf[i];
				buf[i] = buf[k];
				buf[k] = temp;
			}
			
		}
	}
	
	temp = 0;
	for(i=2; i<8; i++)
		temp = temp+buf[i];
	tpad_default_val = temp/6;
	
	printf("tpad_default_val:%d\n",tpad_default_val);
	
	if(tpad_default_val>TPAD_ARR_MAX_VAL/2)
		return 1;
	
	return 0;
}
u16 TPAD_Get_Val(void)
{
	TPAD_Reset();
	
	while(TIM_GetFlagStatus(TIM5,TIM_FLAG_CC2) == RESET)
	{
		if(TIM_GetCounter(TIM5)>TPAD_ARR_MAX_VAL-500)
			return TIM_GetCounter(TIM5);
	}
	return TIM_GetCounter(TIM5);
}
u16 TPAD_Get_Max_Val(u8 n)
{
	u16 temp = 0;
	u16 res = 0;
	
	while(n--)
	{
		temp = TPAD_Get_Val();		
		if(temp > res)
			res = temp;
	}
	return res ;
}
void TPAD_Reset(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);//放电
	
	delay_ms(5);
	
	TIM_SetCounter(TIM5,0);
	TIM_ClearITPendingBit(TIM5,TIM_IT_CC2|TIM_IT_Update);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}

u8 TPAD_Scan(u8 mode)
{
	static u8 tpad_up = 0;
	u16 temp ;
	u8 flag = 0;
	u8 n = 3; //默认采样次数为3次 选最大值
	if(mode)
	{
		tpad_up = 0;
		n = 6;
	}
	
	temp = TPAD_Get_Max_Val(n);
	
	if(temp > (tpad_default_val+TPAD_GATE_VAL))
	{
		if(tpad_up == 0)
			flag = 1;
//			printf("此次捕获有效,值为：%d",temp);
		tpad_up = 3;
	}
	if(tpad_up)
		tpad_up--;
	return flag;
}
