#include "stm32f10x.h"
#include "test.h"
#include "xj.h"
#include "data_channel.h"
#include "usart.h"
#include "canp_hostcom.h"
#define __EXIT_C__
#include "exit.h"
#include "rc522.h"

u8 Wheel_flag=0;
//extern u8 rx2zt_flag;
/***************************************************************
** ���ܣ�     ��ʱ��2�жϷ������
** ������	  �޲���
** ����ֵ��    ��
****************************************************************/ 
void TIM2_IRQHandler(void)
{ 	
  if(TIM2->SR&0X0001)//����ж�
 {
	
	 gd= (Get_Host_UpTrack( TRACK_H8) )&0xff;  // ��ȡѭ������
	 
	if(Track_Flag)
	{
		Track();					//ѭ������
		
	}
	
	else if(G_Flag&&(MP>tempMP))			//�������5CM	  37
	{
		G_Flag=0;
		Stop_Flag=3;
		STOP();
  //  U1SendChar(0x55);
	}
	else if(B_Flag&&(MP>tempMP))			//�������5CM
	{
		B_Flag=0;
		Stop_Flag=3;
		STOP();
	}
	else if(L_Flag)				
	{
		if(MP>550)				  //�������50CM
		{
			L_Flag=0;
			Wheel_flag=0;
			Stop_Flag=2;
		    STOP();
			Host_Close_UpTrack( );  // �ر�Ѱ�������ϴ�
		}
		else
		{
		 if(!(gd &0X10)) //�ҵ�ѭ���ߣ�ֹͣ
			{	
				if(Wheel_flag)
				{
					L_Flag=0;
					Wheel_flag=0;
					Stop_Flag=2;
					Host_Close_UpTrack( );  // �ر�Ѱ�������ϴ�
					STOP();
				}
			}
			else if( gd == 0Xff) Wheel_flag=1;
			
		}
	}
	else if(R_Flag)			
	{
		if(MP>550)				  //�������50CM
		{
			R_Flag=0;
			Wheel_flag=0;
			Stop_Flag=2;
		    STOP();
			Host_Close_UpTrack( );  // �ر�Ѱ�������ϴ�
		}
		else
		{
		 if(!(gd &0X08)) //�ҵ�ѭ���ߣ�ֹͣ
			{	
				if(Wheel_flag)
				{
					R_Flag=0;
					Wheel_flag=0;
					Stop_Flag=2;
		       STOP();
					Host_Close_UpTrack( );  // �ر�Ѱ�������ϴ�
				}
			}
			else if( gd == 0Xff) Wheel_flag=1;	

		}
	}
			    				   				     	    	
	}				   
	TIM2->SR&=~(1<<0);//����жϱ�־λ 	    
}
/***************************************************************
** ���ܣ�     ��ʱ��2�жϳ�ʼ��
** ������	  arr���Զ���װֵ
**			  psc��ʱ��Ԥ��Ƶ��
** ����ֵ��    ��
****************************************************************/
void Timer2_Init(u16 arr,u16 psc)
{
	
// 	RCC->APB1ENR|=1<<0;//TIM2ʱ��ʹ��    
//  	TIM2->ARR=arr;  //�趨�������Զ���װֵ
// 	TIM2->PSC=psc;  //Ԥ��Ƶ��7200,�õ�10Khz�ļ���ʱ��
// 	//����������Ҫͬʱ���òſ���ʹ���ж�
// 	TIM2->DIER|=1<<0;   //��������ж�				
// 	TIM2->DIER|=1<<6;   //�������ж�	   
// 	TIM2->CR1|=0x01;    //ʹ�ܶ�ʱ��2
// //  	MY_NVIC_Init(1,2,TIM2_IRQChannel,3);//��ռ1�������ȼ�2����3		
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM2��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM2�ж�,��������ж�

	//�ж����ȼ�NVIC����
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx	ENABLE
  

}

/***************************************************************
** ���ܣ�     ��ʱ��4�жϷ�����
** ������	  �޲���
** ����ֵ��    ��
****************************************************************/
void TIM4_IRQHandler(void)
{ 	    		  			    
	if(TIM4->SR&0X0001)         // ����ж�
	{	 
		 rxd1_timer4_1ms++;
		 if(rxd1_timer4_1ms>5)	   // 5msʱ���ѵ�������֡�������
		 {
		 	  rxd1_timer4_flag=1;
			  TIM4->CR1&=~(0x01);       // ��ʹ�ܶ�ʱ��4
		     Wifi_Rx_flag =1;
		 }
    }
		 					   
	TIM4->SR&=~(1<<0);          // ����жϱ�־λ 	    
}

/***************************************************************
** ���ܣ�     ��ʱ��4�жϳ�ʼ��
** ������	  arr���Զ���װֵ
**			  psc��ʱ��Ԥ��Ƶ��
** ����ֵ��    ��
****************************************************************/
void Timer4_Init(u16 arr,u16 psc)
{
// 	RCC->APB1ENR|=1<<2;//TIM4ʱ��ʹ��    
//  	TIM4->ARR=arr;  //�趨�������Զ���װֵ
// 	TIM4->PSC=psc;  //Ԥ��Ƶ��7200,�õ�10Khz�ļ���ʱ��
// 	//����������Ҫͬʱ���òſ���ʹ���ж�
// 	TIM4->DIER|=1<<0;   //��������ж�				
// 	TIM4->DIER|=1<<6;   //�������ж�	   
// 	TIM4->CR1|=0x01;    //ʹ�ܶ�ʱ��4
//   	MY_NVIC_Init(1,1,TIM4_IRQChannel,3);//��ռ1�������ȼ�2����3	
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM4��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM4�ж�,��������ж�

	//�ж����ȼ�NVIC����
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx		
}


