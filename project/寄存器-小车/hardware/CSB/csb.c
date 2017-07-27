
#include "sys.h"
#include "delay.h"		
#include "csb.h"	
#include "test.h"


// 变量 
u32 status=0;						// 计数值
u8 tx=0;
float  real_time;	 				// 读回值

u16	dis;						// 距离计算值

/***************************************************************
** 功能：     计算超声波所测得距离
** 参数：	  无参数
** 返回值：    无
****************************************************************/ 	
void jscsb()
{
   real_time = status ;
	
   real_time=(float)real_time*1.72-20;     // 计算距离	定时10us，S=Vt/2（减20是误差补尝）
   dis=(u16)real_time;

}

/***************************************************************
** 功能：     外部中断9-5服务程序 使用了中断9
** 参数：	  无参数
** 返回值：    无
****************************************************************/
void EXTI9_5_IRQHandler(void)
{ 
	if(INT0==0)	
	{
	  TIM3->CR1&=~(0x01);                   // 关使能定时器3
	  jscsb();
     
	}
   
	  									 
	EXTI->PR=1<<9;                       // 清除LINE9上的中断标志位
}

/***************************************************************
** 功能：     外部中断15-10服务程序 使用了中断10
** 参数：	  无参数
** 返回值：    无
****************************************************************/
void EXTI15_10_IRQHandler(void)
{ 
	  if(SPEED==0)
	  {
	    if(tx>5)
		{
		  tx=0;
		  CodedDisk++;
	      MP++;
		}
	  	else tx++; 
	  }
	   
	   EXTI->PR=1<<10;  //清除LINE10上的中断标志位
	 
}

/***************************************************************
** 功能：     外部中断初始化
** 参数：	  无参数
** 返回值：    无
****************************************************************/
void EXTIX_Init(void)
{

	GPIOB->CRH&=0XFFFFF00F;  // PB9/PB10设置成输入	  
	GPIOB->CRH|=0X00000880;
	GPIOB->ODR|=3<<9;   

	GPIOE->CRL&=0XFFFFFFF0;  // PE0设置成输出	  
	GPIOE->CRL|=0X00000003; 				   
	GPIOE->ODR|=1<<0;	     // PE0上拉

	Ex_NVIC_Config(GPIO_B,9,FTIR); //下降沿触发
	Ex_NVIC_Config(GPIO_B,10,FTIR); //下降沿触发
	MY_NVIC_Init(2,2,EXTI9_5_IRQChannel,2);    //抢占2，子优先级2，组2	
	MY_NVIC_Init(2,3,EXTI15_10_IRQChannel,2);  //抢占2，子优先级3，组2   
}

/***************************************************************
** 功能：     定时器3中断服务函数
** 参数：	  无参数
** 返回值：    无
****************************************************************/
void TIM3_IRQHandler(void)
{ 	    		  			    
	if(TIM3->SR&0X0001)         // 溢出中断
	{	 
	 status++; 	

    }
		 					   
	TIM3->SR&=~(1<<0);          // 清除中断标志位 	    
}

/***************************************************************
** 功能：     定时器3中断初始化
** 参数：	  arr：自动重装值
      		  psc：时钟预分频数
** 返回值：    无
****************************************************************/
void Timer3_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<1;         // TIM3时钟使能    
 	TIM3->ARR=arr;              // 设定计数器自动重装值//刚好1ms    
	TIM3->PSC=psc;              // 预分频器7200,得到10Khz的计数时钟
	
	/*这两个东东要同时设置才可以使用中断*/
	TIM3->DIER|=1<<0;           // 允许更新中断				
	TIM3->DIER|=1<<6;           // 允许触发中断
		  							    
//	TIM3->CR1|=0x01;            // 使能定时器3
  	MY_NVIC_Init(1,3,TIM3_IRQChannel,3);//抢占1，子优先级3，组3									 
}
/***************************************************************
** 功能：     超声波发生函数
** 参数：	  无参数
** 返回值：    无
****************************************************************/ 
void tran(void)
{
  

    CSB_TX = 1;            
	delay_us(3) ;
    CSB_TX = 0;

	TIM3->CR1|=0x01;         // 使能定时器3
    TIM3->SR&=~(1<<0);       // 清除中断标志位    
    status  = 0;			 // 定时器清零

	delay_ms(30) ;			 //等待一段时间，等待发送超声波控制信号
	CSB_TX = 1;
   delay_ms(5);

}
