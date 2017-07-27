#include "timer.h"
#include"lcd1602.h"

extern char shi,fen,miao,nian1,nian2,yue,ri;

/*******************************************************************************
* �� �� ��         : TIM3_IRQHandler()
* ��������		   : ��ʱ������ʱ������ж�
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void TIM3_IRQHandler(void)
{ 		    		  			    
	if(TIM3->SR&0X0001)//����ж�
	{
		miao++;
		LcdWriteSfm(10,miao);		   //�� �� ʵʱ������ʾ��Һ����
		if(miao >= 59)
		{
			miao = -1;
			fen++;
			LcdWriteSfm(7,fen);
			if(fen >= 59)
			{
				fen = -1;
				shi++;
				LcdWriteSfm(4,shi);
				if(shi >= 23)
				{
					shi = -1;
					ri++;
					LcdWriteNyr(11,ri);
					if(ri >= 30)
					{
						ri = -1;
						yue++;
						LcdWriteNyr(8,yue);
						if(yue >= 11)
						{
							yue = -1;
							nian1++;
							LcdWriteNyr(5,nian1);
							if(nian1 >= 98)
							{
								nian1 = -1;
								nian2++;
								LcdWriteNyr(3,nian2);
								if(nian2 >= 98)
								{
									nian2 = -1;
								}
							}
						}
					}
				}
			}
		}
				    				   				     	    	
	}				   
	TIM3->SR&=~(1<<0);//����жϱ�־λ 	    
}
/*******************************************************************************
* �� �� ��         : TimerInit
* ��������		   : ��ʱ�����ò��ҳ�ʼ��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void TimerInit(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<1;//TIM3ʱ��ʹ��    
 	TIM3->ARR=arr;  //�趨�������Զ���װֵ//�պ�1ms    
	TIM3->PSC=psc;  //Ԥ��Ƶ��7200,�õ�10Khz�ļ���ʱ��
	//����������Ҫͬʱ���òſ���ʹ���ж�
	TIM3->DIER|=1<<0;   //��������ж�				
	TIM3->DIER|=1<<6;   //�������ж�
	TIM3->CR1=0x8000;   //ARPEʹ��	   
	TIM3->CR1|=0x01;    //ʹ�ܶ�ʱ��3
  	MY_NVIC_Init(1,3,TIM3_IRQChannel,2);//��ռ1�������ȼ�3����2									 
}
