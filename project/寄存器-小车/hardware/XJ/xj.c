
#include "usart.h"	   
#include "xj.h"
#include "djqd.h"
#include "test.h"
#include "hw.h"
u8 gd;
u8 black_cnt = 0;		//D?3��1yo��??��?��y??��y
/***************************************************************
** ���ܣ�     ѭ���˿ڳ�ʼ��
** ������	  �޲���
** ����ֵ��    ��
****************************************************************/
void XJ_Init()
{
	GPIOA->CRL&=0X00000000;	  
	GPIOA->CRL|=0X88888888;//PA0.PA1.PA2.PA3.PA4.PA5.PA6.PA7 ���ó����� 				   
	GPIOA->ODR|=0X00FF;	   //PA0.PA1.PA2.PA3.PA4.PA5.PA6.PA7����
}
/***************************************************************
** ���ܣ�     ѭ������
** ������	  �޲���
** ����ֵ��    ��
****************************************************************/
void Track(void)
{
	gd=GPIOA->IDR&0Xff;
	if((gd==0xff)||(gd == 0x00))	//ѭ����ȫ�� ֹͣ
	{	
		STOP();
		Track_Flag=0;
		Stop_Flag=1;

		if(gd == 0x00)
		{
			black_cnt += 1;
			Car_Run(black_cnt);
			
			if(black_cnt > 3)
				GO_MP(30);
		}
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
				LSpeed=Car_Spend+10;
				RSpeed=Car_Spend-40;
				Line_Flag=0;
			}
			if(gd==0XF9||gd==0XFD)//3���м�3��2��������⵽���ߣ���΢�ҹ�
			{ 
				 LSpeed=Car_Spend+20;
				 RSpeed=Car_Spend-60;				 
				 Line_Flag=0;
			}
			if(gd==0XFC)//4���м�2��1��������⵽���ߣ�ǿ�ҹ�
			{ 
				LSpeed=Car_Spend+40;
				RSpeed=Car_Spend-100;
				 Line_Flag=0;
			}
			if(gd==0XFE)//5�����ұ�1��������⵽���ߣ���ǿ�ҹ�
			{ 
				 LSpeed=Car_Spend+50;
				 RSpeed=Car_Spend-120;			
				 Line_Flag=1;
			}
		}
		if(Line_Flag!=1)
		{
			if(gd==0XCF)//6���м�6��5��������⵽���ߣ�΢���		 
			{ 
				 RSpeed=Car_Spend+10;
				 LSpeed=Car_Spend-40;
				 Line_Flag=0;
			} 
			if(gd==0X9F||gd==0XDF)//7���м�7��6��������⵽���ߣ���΢���		 
			{ 
				RSpeed=Car_Spend+20;
				 LSpeed=Car_Spend-60;
				 Line_Flag=0;
			} 			  
			if(gd==0X3F||gd==0XBF)//8���м�8��7��������⵽���ߣ�ǿ���		 
			{ 
				 RSpeed=Car_Spend+40;
				 LSpeed=Car_Spend-100;
				 Line_Flag=0;
			} 
			if(gd==0X7F)//9������8��������⵽���ߣ���ǿ���		 
			{ 
				 RSpeed=Car_Spend+50;
				 LSpeed=Car_Spend-120;
				 Line_Flag=2;
			}
		}
		if(gd==0xFF)   //ѭ����ȫ��
		{
			if(Line_Flag==0)
			{
				if(count++>1000)
				{
					count=0;
		            STOP();
					Track_Flag=0;
					Stop_Flag=4;
				}		
			}
		}
		else	
			count=0;
	   	if(!Track_Flag==0)
		{
			Control(LSpeed,RSpeed);
		}
	}
}

void Car_Run(u8 cnt)
{
	switch(cnt)
	{
		case 1:
		{
			MP = 0;
			STOP();
			GO_MP(30);
			break;
		}
		case 2:
		{
			MP = 0;
			STOP();
			GO_MP(100);
			RIGHT_MP(110);
			break;
		}
		case 3:
		{
			MP = 0;
			STOP();
			Transmition(HW_G,6);
			delay_ms(1500);
			delay_ms(1500);
			Transmition(HW_K,6);
			delay_ms(500);
			BACK_MP(200);
			RIGHT_MP(110);
			break;
		}
	}
}

void GO_MP(u16 mp)
{
	MP = 0;
	G_Flag = 1;
	tempMP = mp;
	Control(90,90);
	while(!G_Flag == 0);
}
void BACK_MP(u16 mp)
{
	MP = 0;
	B_Flag = 1;
	tempMP = mp;
	Control(-51,-50);
	while(!B_Flag == 0);
}
void RIGHT_MP(u16 mp)
{
	MP = 0;
	R_Flag = 1;
	tempMP = mp;
	Control(50,-50);
	while(!R_Flag == 0);
}
void LEFT_MP(u16 mp)
{
	MP = 0;
	L_Flag = 1;
	tempMP = mp;
	Control(-50,50);
	while(!L_Flag == 0);
}




