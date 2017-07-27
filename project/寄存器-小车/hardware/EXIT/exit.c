
#include "exit.h"
#include "test.h"
#include "xj.h"

u8 Wheel_flag=0;

/***************************************************************
** ���ܣ�     ��ʱ��2�жϷ������
** ������	  �޲���
** ����ֵ��    ��
****************************************************************/ 
void TIM2_IRQHandler(void)
{ 		    		  			    
  if(TIM2->SR&0X0001)//����ж�
 {

	if(Track_Flag)
	{
		Track();					//ѭ������
	}
	if(G_Flag&&(MP>tempMP))			//�������5CM	  37
	{
		G_Flag=0;
		Stop_Flag=3;
		STOP();
	}
	if(B_Flag&&(MP>tempMP))			//�������5CM
	{
		B_Flag=0;
		Stop_Flag=3;
		STOP();
	}
	if(L_Flag)				
	{
		if(MP>550)				  //�������50CM
		{
			L_Flag=0;
			Wheel_flag=0;
		    STOP();
		}
		else
		{
		  	if((GPIOA->IDR&0Xff)==0XE7||(GPIOA->IDR&0Xff)==0XF3) //�ҵ�ѭ���ߣ�ֹͣ
			{	
				if(Wheel_flag)
				{
					L_Flag=0;
					Wheel_flag=0;
					Stop_Flag=2;
					STOP();
				}
			}
			if((GPIOA->IDR&0Xff)==0xFF)Wheel_flag=1;

		}
	}
	if(R_Flag)			
	{
		if(MP>550)				  //�������50CM
		{
			R_Flag=0;
			Wheel_flag=0;
		    STOP();
		}
		else
		{
		  	if((GPIOA->IDR&0Xff)==0XE7||(GPIOA->IDR&0Xff)==0XCF)   //�ҵ�ѭ���ߣ�ֹͣ
			{	
				if(Wheel_flag)
				{
					R_Flag=0;
					Wheel_flag=0;
					Stop_Flag=2;
		            STOP();
				}
			}
			if((GPIOA->IDR&0Xff)==0xFF)Wheel_flag=1;

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
	RCC->APB1ENR|=1<<0;//TIM3ʱ��ʹ��    
 	TIM2->ARR=arr;  //�趨�������Զ���װֵ
	TIM2->PSC=psc;  //Ԥ��Ƶ��7200,�õ�10Khz�ļ���ʱ��
	//����������Ҫͬʱ���òſ���ʹ���ж�
	TIM2->DIER|=1<<0;   //��������ж�				
	TIM2->DIER|=1<<6;   //�������ж�	   
	TIM2->CR1|=0x01;    //ʹ�ܶ�ʱ��2
  	MY_NVIC_Init(1,2,TIM2_IRQChannel,3);//��ռ1�������ȼ�2����3									 
}
