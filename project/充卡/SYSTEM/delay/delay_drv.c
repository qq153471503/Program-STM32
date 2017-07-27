
#include "stm32f10x.h"
#include "timer.h"

#define __DELAY_DRV_C__
#include "delay_drv.h"

void Delay_us_Init(void)
{

   TIM5_Int_Init( 0, 71);  // ����Ƶ

}

void delay_us(u32 nus)
{		
	u32 fu_nus =0;
	
	 fu_nus = nus;  //72M ϵͳ��Ƶ
	TIM_SetAutoreload( TIM5, fu_nus);  // ������װ��ֵ
	TIM5->CNT =0;
	TIM_SetCounter(TIM5 , 0);   // ���CNT
	TIM_ClearFlag( TIM5, TIM_FLAG_Update);
	TIM_Cmd(TIM5, ENABLE);  //ʹ��TIM5
	
	while( !(TIM_GetFlagStatus(TIM5, TIM_FLAG_Update)));
	
	TIM_SetCounter(TIM5 , 0);   // ���CNT
	TIM_Cmd(TIM5, DISABLE);  //ʹ��TIM5
   
}

void sleep(unsigned long c)
{
	while(c--);
}

void delay_ms(u32 c)
{
	delay_ms_const = c;
	while(delay_ms_const);
}

void delay_ms_set(u32 c)
{
	delay_ms_const = c;
}

u32 delay_ms_getlost(void)
{
	return delay_ms_const;
}

u8 delay_ms_endcheck(void)
{
	return delay_ms_const ? 0:1;
}

u32 gt_get(void)
{
	return global_times;
}

u32 gt_get_sub(u32 c)
{
	if(c > global_times)
		c -= global_times;
	else
		c = 0;
	return c;
}

//end file

