#include "xj.h"
#include "delay.h"

u16 cnt = 0;
u16 cnt2 = 5;
u16 cnt3 = 5;
u16 cnt4 = 2;
u8 i,k;
u8 ad1 = 185;		//ÓÒ½ÅÐÞÕý²ÎÊý½ÃÕý
u8 ad2 = 117;		//×ó½ÅÐÞÕý²ÎÊý½ÃÕý

void XJ_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
}

void run(void)
{
		for(i=145,k=151; i>=135,k>=141; i--,k--)					//ÓÒ½ÅÍá
		{
			TIM_SetCompare4(TIM2,i);
			TIM_SetCompare1(TIM2,k);
			delay_ms(10);
		}
		for(i=185,k=185; k>=115,i>=115; i--,k--)
		{
			TIM_SetCompare2(TIM2,i);
			TIM_SetCompare3(TIM2,k);
			delay_ms(6);
		}
		for(i=115; i<=118; i++)
		{
		//	ad2 = 117;								//×ó½ÅÄÚÆ«
			TIM_SetCompare2(TIM2,i);
			delay_ms(8);
		}
		for(i=135,k=141; i<=145,k<=151; i++,k++)		//ÓÒ½ÅÕý
		{
			TIM_SetCompare4(TIM2,i);
			TIM_SetCompare1(TIM2,k);
			delay_ms(10);
		}
		/*/////////////////////////////////*/
		if((xj1==0)&&(xj2==0)&&(xj3==0))
			ad1 = 185;
		else if((xj1==1)&&(xj2==0)&&(xj3==0))
			ad1 = 181;
		else if((xj1==1)&&(xj2==1)&&(xj3==0))
			ad1 = 177;
		else if((xj1==1)&&(xj2==1)&&(xj3==1))
			ad1 = 175;
		/*/////////////////////////////////*/
		for(i=145,k=151; i<=155,k<=161; i++,k++)		//×ó½ÅÍá
		{
			TIM_SetCompare1(TIM2,k);
			TIM_SetCompare4(TIM2,i);
			delay_ms(10);
		}
		for(i=115,k=115; i<=185,k<=185; i++,k++)
		{	
			TIM_SetCompare2(TIM2,i);
			TIM_SetCompare3(TIM2,k);
			delay_ms(6);
		}
		for(i=185; i>=ad1; i--)
		{
			TIM_SetCompare3(TIM2,ad1);
			delay_ms(30);
		}
		/*//////////////////////////////////////////*/
		for(i=155,k=161; i>=145,k>=151; i--,k--)					//×ó½ÅÕý
		{
			TIM_SetCompare1(TIM2,k);
			TIM_SetCompare4(TIM2,i);
			delay_ms(10);
		}
		cnt++;
		if(cnt == 0x23)
			swerve();
}
void swerve(void)
{
	while(cnt2--)
	{
		for(i=145,k=151; i>=135,k>=141; i--,k--)	//ÓÒ½ÅÍá
		{
			TIM_SetCompare4(TIM2,i);
			TIM_SetCompare1(TIM2,k);
			delay_ms(10);
		}
		for(i=185,k=185; k>=115,i>=115; i--,k--)
		{
			TIM_SetCompare2(TIM2,i);
			TIM_SetCompare3(TIM2,k);
			delay_ms(6);
		}
		for(i=115; i>=108; i--)						//×ó½ÅÍâÆ«
		{							
			TIM_SetCompare2(TIM2,i);
			delay_ms(100);
		}
		for(i=135,k=141; i<=145,k<=151; i++,k++)	//ÓÒ½ÅÕý
		{
			TIM_SetCompare4(TIM2,i);
			TIM_SetCompare1(TIM2,k);
			delay_ms(10);
		}
		for(i=145,k=151; i<=155,k<=161; i++,k++)	//×ó½ÅÍá
		{
			TIM_SetCompare1(TIM2,k);
			TIM_SetCompare4(TIM2,i);
			delay_ms(10);
		}
		for(i=115,k=115; i<=185,k<=185; i++,k++)
		{	
			TIM_SetCompare2(TIM2,i);
			TIM_SetCompare3(TIM2,k);
			delay_ms(6);
		}
		for(i=185; i>=182; i--)						//ÓÒ½ÅÄÚÆ«
		{
			TIM_SetCompare3(TIM2,i);
			delay_ms(8);
		}
		for(i=155,k=161; i>=145,k>=151; i--,k--)	//×ó½ÅÕý
		{
			TIM_SetCompare1(TIM2,k);
			TIM_SetCompare4(TIM2,i);
			delay_ms(10);
		}
	}
/**====================================================================================**/
		while(cnt3--)
		{
			for(i=145,k=151; i>=135,k>=141; i--,k--)	//ÓÒ½ÅÍá
			{
				TIM_SetCompare4(TIM2,i);
				TIM_SetCompare1(TIM2,k);
				delay_ms(10);
			}
			for(i=185,k=185; k>=115,i>=115; i--,k--)
			{
				TIM_SetCompare2(TIM2,i);
				TIM_SetCompare3(TIM2,k);
				delay_ms(6);
			}
			for(i=115; i>=108; i--)						//×ó½ÅÍâÆ«
			{							
				TIM_SetCompare2(TIM2,i);
				delay_ms(100);
			}
			for(i=135,k=141; i<=145,k<=151; i++,k++)	//ÓÒ½ÅÕý
			{
				TIM_SetCompare4(TIM2,i);
				TIM_SetCompare1(TIM2,k);
				delay_ms(10);
			}
			for(i=145,k=151; i<=155,k<=161; i++,k++)	//×ó½ÅÍá
			{
				TIM_SetCompare1(TIM2,k);
				TIM_SetCompare4(TIM2,i);
				delay_ms(10);
			}
			for(i=115,k=115; i<=185,k<=185; i++,k++)
			{	
				TIM_SetCompare2(TIM2,i);
				TIM_SetCompare3(TIM2,k);
				delay_ms(6);
			}
			for(i=185; i>=175; i--)						//ÓÒ½ÅÄÚÆ«
			{
				TIM_SetCompare3(TIM2,i);
				delay_ms(8);
			}
			for(i=155,k=161; i>=145,k>=151; i--,k--)	//×ó½ÅÕý
			{
				TIM_SetCompare1(TIM2,k);
				TIM_SetCompare4(TIM2,i);
				delay_ms(10);
			}
		}
/**==============================================================================**/
		while(cnt4--)
		{
			for(i=145,k=151; i>=135,k>=141; i--,k--)	//ÓÒ½ÅÍá
			{
				TIM_SetCompare4(TIM2,i);
				TIM_SetCompare1(TIM2,k);
				delay_ms(10);
			}
			for(i=185,k=185; k>=115,i>=115; i--,k--)
			{
				TIM_SetCompare2(TIM2,i);
				TIM_SetCompare3(TIM2,k);
				delay_ms(6);
			}
			for(i=115; i>=108; i--)						//×ó½ÅÍâÆ«
			{							
				TIM_SetCompare2(TIM2,i);
				delay_ms(100);
			}
			for(i=135,k=141; i<=145,k<=151; i++,k++)	//ÓÒ½ÅÕý
			{
				TIM_SetCompare4(TIM2,i);
				TIM_SetCompare1(TIM2,k);
				delay_ms(10);
			}
			for(i=145,k=151; i<=155,k<=161; i++,k++)	//×ó½ÅÍá
			{
				TIM_SetCompare1(TIM2,k);
				TIM_SetCompare4(TIM2,i);
				delay_ms(10);
			}
			for(i=115,k=115; i<=185,k<=185; i++,k++)
			{	
				TIM_SetCompare2(TIM2,i);
				TIM_SetCompare3(TIM2,k);
				delay_ms(6);
			}
			for(i=185; i>=182; i--)						//ÓÒ½ÅÄÚÆ«
			{
				TIM_SetCompare3(TIM2,i);
				delay_ms(8);
			}
			for(i=155,k=161; i>=145,k>=151; i--,k--)	//×ó½ÅÕý
			{
				TIM_SetCompare1(TIM2,k);
				TIM_SetCompare4(TIM2,i);
				delay_ms(10);
			}
		}
}


