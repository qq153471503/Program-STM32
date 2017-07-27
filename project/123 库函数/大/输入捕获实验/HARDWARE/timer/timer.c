#include "timer.h"


void Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);
	
	TIM_InitStructure.TIM_Period = arr;
	TIM_InitStructure.TIM_Prescaler = psc;
	TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM5,&TIM_InitStructure);
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICPrescaler = 0;
	TIM_ICInitStructure.TIM_ICFilter = 0;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM5,&TIM_ICInitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC1,ENABLE);
	
	TIM_Cmd(TIM5,ENABLE);	
}

u8 TIM5CH1_CAPTURE_STATE = 0;
u16 TIM5CH1_CAPTURE_VALUE;

void TIM5_IRQHandler(void)
{
	if((TIM5CH1_CAPTURE_STATE&0x80) == 0)
	{
		if(TIM_GetITStatus(TIM5,TIM_IT_Update) != RESET)
		{
			if(TIM5CH1_CAPTURE_STATE&0x40)
			{
				if((TIM5CH1_CAPTURE_STATE&0x3f) == 0x3f)
				{
					TIM5CH1_CAPTURE_STATE |= 0x80;
					TIM5CH1_CAPTURE_VALUE = 0xffff;
				}else 
				{
					TIM5CH1_CAPTURE_STATE++;
				}
			}
		}
	}
	
	if(TIM_GetITStatus(TIM5,TIM_IT_CC1) != RESET) 
	{
		if(TIM5CH1_CAPTURE_STATE&0x40)
		{
			TIM5CH1_CAPTURE_STATE |= 0x80;
			TIM5CH1_CAPTURE_VALUE = TIM_GetCapture1(TIM5);
			
			TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising);
		}else 
		{
			TIM5CH1_CAPTURE_STATE = 0;
			TIM5CH1_CAPTURE_VALUE = 0;
			TIM_SetCounter(TIM5,0);
			TIM5CH1_CAPTURE_STATE |= 0X40;
			TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling);
		}
	}
	
	TIM_ClearITPendingBit(TIM5,TIM_IT_CC1|TIM_IT_Update);
}
