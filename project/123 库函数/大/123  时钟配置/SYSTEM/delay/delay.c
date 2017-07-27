#include "delay.h"

static u8  fac_us=0;							//us延时倍乘数			   
static u16 fac_ms=0;							//ms延时倍乘数,在ucos下,代表每个节拍的ms数

//延时函数初始化
void delay_init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//SysTick时钟源为AHB时钟除以8 64Mhz/8=8Mhz
	fac_us=SystemCoreClock/8000000;	
	fac_ms=(u16)fac_us*1000;
}
//微秒级别延时函数
//nus：要延时的us数								   
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; 					 		//预装载值
	SysTick->VAL=0x00;        							//清空当前计数
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;			//使能定时器
	do
	{
		temp=SysTick->CTRL;								//等待定时时间到达
	}while((temp&0x01)&&!(temp&(1<<16)));		
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;			//失能定时器
	SysTick->VAL =0X00;      							//清空当前值寄存器
}
//毫秒级别延时函数
//nms：要延时的ms数
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

