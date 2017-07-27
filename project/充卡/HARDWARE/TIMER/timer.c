#include "timer.h"
#include "canp_hostcom.h"
#include "data_channel.h"
#include "syn7318.h"
#include "test.h"

void TIM5_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
// 	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM5��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM5,TIM_IT_Update, DISABLE ); //ʹ��ָ����TIM5�ж�,��������ж�

	

	TIM_Cmd(TIM5, DISABLE);  //ʹ��TIMx					 
}
// //��ʱ��5�жϷ������
// void TIM5_IRQHandler(void)   //TIM5�ж�
// {
// 	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)  //���TIM5�����жϷ������
// 		{
// 		   TIM_ClearITPendingBit(TIM5, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
// 		   
// 		}
// }

void TIM6_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;


	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM6��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM6,TIM_IT_Update, ENABLE ); //ʹ��ָ����TIM6�ж�,��������ж�

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); //�жϷ��� 0
	//�ж����ȼ�NVIC����
 	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;  //TIM6�ж�
 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
 	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
	
	TIM_Cmd(TIM6, ENABLE);  //ʹ��TIMx					 
}
 //��ʱ��6�жϷ������
 void TIM6_IRQHandler(void)   //TIM5�ж�
 {
 	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)  //���TIM6�����жϷ������
 		{
 		   TIM_ClearITPendingBit(TIM6, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
 		   
					CanP_Host_Main();     // ��ѯCAN��������
		      CanP_CanTx_Check();
			  
 		}
 }












