#include "stm32f10x.h"
#include "usart.h"	   
#include "xj.h"
#include "key.h"
#include "led.h"
#include "djqd.h"
#include "test.h"
#include "rc522.h"
#include "canp_hostcom.h"
#include "bz.h"
u8 gd,c;
u8 gd_old;
u8 temp=0;

/***************************************************************
** ���ܣ�     ѭ���˿ڳ�ʼ��
** ������	  �޲���
** ����ֵ��    ��
****************************************************************/
void XJ_Init()  // ѭ������ͨ��CAN����
{
// 	RCC->APB2ENR|=1<<2;    //ʹ��PORTAʱ��
//	
//	GPIOA->CRL&=0XFFFFFFF0;	  
// 	GPIOA->CRL|=0X00000003;//PA0 ���ó���� 				   
// 	GPIOA->ODR|=1<<0;	   //P���Ϊ��
//	
//  Track_Usart2_Close();
	
//	GPIOA->CRL&=0X00000000;	  
// 	GPIOA->CRL|=0X88888888;//PA0 ���ó���� 				   
// 	GPIOA->ODR|=0x00ff;	   //P���Ϊ��	

}

 void Track_Usart2_Open( void)  // ѭ�����ݴ���2 ����ʹ�� ����Ч
{
     TRACK_EN =0;
}

void Track_Usart2_Close( void)  // ѭ�����ݴ���2 �����ֹ 
{
     TRACK_EN =1;
}


/***************************************************************
** ���ܣ�     ѭ������
** ������	  �޲���
** ����ֵ��    ��
****************************************************************/
void Track(void)
{
	temp++;
	c=(gd&0x18);
	if(ka==1&&temp>20)
	{
		temp=0;
		Rc522(guangdang,0);
		Rc522(guangdang,0);
	}	
		
	
	if((gd == 0x00)||(c==0x18&&gd!=0xFF&&(((gd&0x80)!=0x80)||((gd&0x01)!=0x01))))	//ѭ����ȫ�� ֹͣ
	{	
		
		Track_Flag=0;
		Stop_Flag=1;
		STOP();
		Host_Close_UpTrack( );  // �ر�Ѱ�������ϴ�
	}
	else
	{
	   	Stop_Flag=0;

		if(gd==0XE7||gd==0XF7||gd==0XEF)//1���м�3/4��������⵽���ߣ�ȫ������
		{ 
			LSpeed=Car_Spend;
			RSpeed=Car_Spend;

		}

		 if(Line_Flag!=2)
		{		

			if(gd==0XF3||gd==0XFB) //2���м�4��3��������⵽���ߣ�΢�ҹ�
		
			{ 
				LSpeed=Car_Spend+30;
				RSpeed=Car_Spend-30;
				Line_Flag=0;

			}
			else if(gd==0XF9||gd==0XFD)//3���м�3��2��������⵽���ߣ���΢�ҹ�
		
			{ 
				 LSpeed=Car_Spend+40;
				 RSpeed=Car_Spend-60;				 
				 Line_Flag=0;

			}
			else if(gd==0XFC)//4���м�2��1��������⵽���ߣ�ǿ�ҹ�
			{ 
				LSpeed=Car_Spend+50;
				RSpeed=Car_Spend-90;
				 Line_Flag=0;

			}
			else if(gd==0XFE)//5�����ұ�1��������⵽���ߣ���ǿ�ҹ�
			
			{ 
				 LSpeed=Car_Spend+60;
				 RSpeed=Car_Spend-120;			
				 Line_Flag=1;

			}

		}
		
	 if(Line_Flag!=1)
		{

			if(gd==0XCF)//6���м�6��5��������⵽���ߣ�΢���	
      		
			{ 
				 RSpeed=Car_Spend+30;
				 LSpeed=Car_Spend-30;
				 Line_Flag=0;

			} 
			else if(gd==0X9F||gd==0XDF)//7���м�7��6��������⵽���ߣ���΢���		 
			
			{ 
				RSpeed=Car_Spend+40;
				 LSpeed=Car_Spend-60;
				 Line_Flag=0;

			} 			  
			else if(gd==0X3F||gd==0XBF)//8���м�8��7��������⵽���ߣ�ǿ���		 
			{ 
				 RSpeed=Car_Spend+50;
				 LSpeed=Car_Spend-90;
				 Line_Flag=0;

			} 
			else if(gd==0X7F)//9������8��������⵽���ߣ���ǿ���		 
			
			{ 
				 RSpeed=Car_Spend+60;
				 LSpeed=Car_Spend-120;
				 Line_Flag=2;

			}
			
		}

		if(gd==0xFF)   //ѭ����ȫ��
		{
			LSpeed=Car_Spend;
			RSpeed=Car_Spend;	

		 }

	   	if(!Track_Flag==0)
		{
			Control(LSpeed,RSpeed);
		}

	}
	
	if( Track_Flag==0)
		{
			Host_Close_UpTrack( );  // �ر�Ѱ�������ϴ�	
		}
	
}


void Xj_Track_Test(void)
{
//    u8 track_flag =0;
	 DJ_Init();				 //���������ʼ��
	 PWM_Init();             //PWM��ʼ�� 
	 LED_Init();
	 XJ_Init();				//ѭ����ʼ��
	flag2 =0;
//	uart1_init(115200);	 
  uart2_init(115200, 0);	    // ���ڳ�ʼ��Ϊ115200 // Ĭ�Ͽ��ж�	
	Track_Usart2_Open();
	delay_ms(100);
	Track_Flag =1;
	while(1)
		{
    
	if( flag2 )  
	  {            
	     gd = USART2_RX_BUF[0];	
		   flag2 =0;
			 LED0 =!LED0;
	   }
	 
			Track();
    			
			
    }

}



