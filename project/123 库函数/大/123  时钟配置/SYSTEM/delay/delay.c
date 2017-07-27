#include "delay.h"

static u8  fac_us=0;							//us��ʱ������			   
static u16 fac_ms=0;							//ms��ʱ������,��ucos��,����ÿ�����ĵ�ms��

//��ʱ������ʼ��
void delay_init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//SysTickʱ��ԴΪAHBʱ�ӳ���8 64Mhz/8=8Mhz
	fac_us=SystemCoreClock/8000000;	
	fac_ms=(u16)fac_us*1000;
}
//΢�뼶����ʱ����
//nus��Ҫ��ʱ��us��								   
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; 					 		//Ԥװ��ֵ
	SysTick->VAL=0x00;        							//��յ�ǰ����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;			//ʹ�ܶ�ʱ��
	do
	{
		temp=SysTick->CTRL;								//�ȴ���ʱʱ�䵽��
	}while((temp&0x01)&&!(temp&(1<<16)));		
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;			//ʧ�ܶ�ʱ��
	SysTick->VAL =0X00;      							//��յ�ǰֵ�Ĵ���
}
//���뼶����ʱ����
//nms��Ҫ��ʱ��ms��
void delay_ms(u16 nms)
{	 		
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;				
	SysTick->VAL =0x00;							
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	
	SysTick->VAL =0X00;       					    
} 

