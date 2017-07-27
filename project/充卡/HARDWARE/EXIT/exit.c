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
** 功能：     定时器2中断服务程序
** 参数：	  无参数
** 返回值：    无
****************************************************************/ 
void TIM2_IRQHandler(void)
{ 	
  if(TIM2->SR&0X0001)//溢出中断
 {
	
	 gd= (Get_Host_UpTrack( TRACK_H8) )&0xff;  // 获取循迹数据
	 
	if(Track_Flag)
	{
		Track();					//循迹程序
		
	}
	
	else if(G_Flag&&(MP>tempMP))			//距离大于5CM	  37
	{
		G_Flag=0;
		Stop_Flag=3;
		STOP();
  //  U1SendChar(0x55);
	}
	else if(B_Flag&&(MP>tempMP))			//距离大于5CM
	{
		B_Flag=0;
		Stop_Flag=3;
		STOP();
	}
	else if(L_Flag)				
	{
		if(MP>550)				  //距离大于50CM
		{
			L_Flag=0;
			Wheel_flag=0;
			Stop_Flag=2;
		    STOP();
			Host_Close_UpTrack( );  // 关闭寻迹数据上传
		}
		else
		{
		 if(!(gd &0X10)) //找到循迹线，停止
			{	
				if(Wheel_flag)
				{
					L_Flag=0;
					Wheel_flag=0;
					Stop_Flag=2;
					Host_Close_UpTrack( );  // 关闭寻迹数据上传
					STOP();
				}
			}
			else if( gd == 0Xff) Wheel_flag=1;
			
		}
	}
	else if(R_Flag)			
	{
		if(MP>550)				  //距离大于50CM
		{
			R_Flag=0;
			Wheel_flag=0;
			Stop_Flag=2;
		    STOP();
			Host_Close_UpTrack( );  // 关闭寻迹数据上传
		}
		else
		{
		 if(!(gd &0X08)) //找到循迹线，停止
			{	
				if(Wheel_flag)
				{
					R_Flag=0;
					Wheel_flag=0;
					Stop_Flag=2;
		       STOP();
					Host_Close_UpTrack( );  // 关闭寻迹数据上传
				}
			}
			else if( gd == 0Xff) Wheel_flag=1;	

		}
	}
			    				   				     	    	
	}				   
	TIM2->SR&=~(1<<0);//清除中断标志位 	    
}
/***************************************************************
** 功能：     定时器2中断初始化
** 参数：	  arr：自动重装值
**			  psc：时钟预分频数
** 返回值：    无
****************************************************************/
void Timer2_Init(u16 arr,u16 psc)
{
	
// 	RCC->APB1ENR|=1<<0;//TIM2时钟使能    
//  	TIM2->ARR=arr;  //设定计数器自动重装值
// 	TIM2->PSC=psc;  //预分频器7200,得到10Khz的计数时钟
// 	//这两个东东要同时设置才可以使用中断
// 	TIM2->DIER|=1<<0;   //允许更新中断				
// 	TIM2->DIER|=1<<6;   //允许触发中断	   
// 	TIM2->CR1|=0x01;    //使能定时器2
// //  	MY_NVIC_Init(1,2,TIM2_IRQChannel,3);//抢占1，子优先级2，组3		
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
	
	//定时器TIM2初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //使能指定的TIM2中断,允许更新中断

	//中断优先级NVIC设置
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM2, ENABLE);  //使能TIMx	ENABLE
  

}

/***************************************************************
** 功能：     定时器4中断服务函数
** 参数：	  无参数
** 返回值：    无
****************************************************************/
void TIM4_IRQHandler(void)
{ 	    		  			    
	if(TIM4->SR&0X0001)         // 溢出中断
	{	 
		 rxd1_timer4_1ms++;
		 if(rxd1_timer4_1ms>5)	   // 5ms时间已到，数据帧接收完成
		 {
		 	  rxd1_timer4_flag=1;
			  TIM4->CR1&=~(0x01);       // 关使能定时器4
		     Wifi_Rx_flag =1;
		 }
    }
		 					   
	TIM4->SR&=~(1<<0);          // 清除中断标志位 	    
}

/***************************************************************
** 功能：     定时器4中断初始化
** 参数：	  arr：自动重装值
**			  psc：时钟预分频数
** 返回值：    无
****************************************************************/
void Timer4_Init(u16 arr,u16 psc)
{
// 	RCC->APB1ENR|=1<<2;//TIM4时钟使能    
//  	TIM4->ARR=arr;  //设定计数器自动重装值
// 	TIM4->PSC=psc;  //预分频器7200,得到10Khz的计数时钟
// 	//这两个东东要同时设置才可以使用中断
// 	TIM4->DIER|=1<<0;   //允许更新中断				
// 	TIM4->DIER|=1<<6;   //允许触发中断	   
// 	TIM4->CR1|=0x01;    //使能定时器4
//   	MY_NVIC_Init(1,1,TIM4_IRQChannel,3);//抢占1，子优先级2，组3	
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能
	
	//定时器TIM4初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM4中断,允许更新中断

	//中断优先级NVIC设置
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM4, ENABLE);  //使能TIMx		
}


