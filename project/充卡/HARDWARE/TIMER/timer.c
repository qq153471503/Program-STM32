#include "timer.h"
#include "canp_hostcom.h"
#include "data_channel.h"
#include "syn7318.h"
#include "test.h"

void TIM5_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
// 	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //时钟使能
	
	//定时器TIM5初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM5,TIM_IT_Update, DISABLE ); //使能指定的TIM5中断,允许更新中断

	

	TIM_Cmd(TIM5, DISABLE);  //使能TIMx					 
}
// //定时器5中断服务程序
// void TIM5_IRQHandler(void)   //TIM5中断
// {
// 	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)  //检查TIM5更新中断发生与否
// 		{
// 		   TIM_ClearITPendingBit(TIM5, TIM_IT_Update  );  //清除TIMx更新中断标志 
// 		   
// 		}
// }

void TIM6_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;


	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //时钟使能
	
	//定时器TIM6初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM6,TIM_IT_Update, ENABLE ); //使能指定的TIM6中断,允许更新中断

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); //中断分组 0
	//中断优先级NVIC设置
 	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;  //TIM6中断
 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
 	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
	
	TIM_Cmd(TIM6, ENABLE);  //使能TIMx					 
}
 //定时器6中断服务程序
 void TIM6_IRQHandler(void)   //TIM5中断
 {
 	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)  //检查TIM6更新中断发生与否
 		{
 		   TIM_ClearITPendingBit(TIM6, TIM_IT_Update  );  //清除TIMx更新中断标志 
 		   
					CanP_Host_Main();     // 查询CAN总线数据
		      CanP_CanTx_Check();
			  
 		}
 }












