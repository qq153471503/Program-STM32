
#include "exit.h"
#include "test.h"
#include "xj.h"

u8 Wheel_flag=0;

/***************************************************************
** 功能：     定时器2中断服务程序
** 参数：	  无参数
** 返回值：    无
****************************************************************/ 
void TIM2_IRQHandler(void)
{ 		    		  			    
  if(TIM2->SR&0X0001)//溢出中断
 {

	if(Track_Flag)
	{
		Track();					//循迹程序
	}
	if(G_Flag&&(MP>tempMP))			//距离大于5CM	  37
	{
		G_Flag=0;
		Stop_Flag=3;
		STOP();
	}
	if(B_Flag&&(MP>tempMP))			//距离大于5CM
	{
		B_Flag=0;
		Stop_Flag=3;
		STOP();
	}
	if(L_Flag)				
	{
		if(MP>550)				  //距离大于50CM
		{
			L_Flag=0;
			Wheel_flag=0;
		    STOP();
		}
		else
		{
		  	if((GPIOA->IDR&0Xff)==0XE7||(GPIOA->IDR&0Xff)==0XF3) //找到循迹线，停止
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
		if(MP>550)				  //距离大于50CM
		{
			R_Flag=0;
			Wheel_flag=0;
		    STOP();
		}
		else
		{
		  	if((GPIOA->IDR&0Xff)==0XE7||(GPIOA->IDR&0Xff)==0XCF)   //找到循迹线，停止
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
	RCC->APB1ENR|=1<<0;//TIM3时钟使能    
 	TIM2->ARR=arr;  //设定计数器自动重装值
	TIM2->PSC=psc;  //预分频器7200,得到10Khz的计数时钟
	//这两个东东要同时设置才可以使用中断
	TIM2->DIER|=1<<0;   //允许更新中断				
	TIM2->DIER|=1<<6;   //允许触发中断	   
	TIM2->CR1|=0x01;    //使能定时器2
  	MY_NVIC_Init(1,2,TIM2_IRQChannel,3);//抢占1，子优先级2，组3									 
}
