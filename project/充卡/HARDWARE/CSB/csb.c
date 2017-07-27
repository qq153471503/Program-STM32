
#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "sys.h"
#include "delay.h"		
#include "csb.h"	
#include "test.h"


// 变量 
u32 status=0;						// 计数值
u8 tx=0,txr=0;
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

# define SPPED_INT_MODE 0   //  1 左侧码盘计数   0右侧

#if SPPED_INT_MODE
/***************************************************************
** 功能：     外部中断15-10服务程序 使用了中断10
** 参数：	  无参数
** 返回值：    无
****************************************************************/
void EXTI15_10_IRQHandler(void)
{ 
	  if(SPEED==0)
	  {
	    if(tx>2)
		{
		  tx=0;
		  CodedDisk++;
	      MP++;
		}
	  	else tx++; 
	  }
	   
	   EXTI->PR=1<<10;  //清除LINE10上的中断标志位
	 
}

#else 

/***************************************************************
** 功能：     外部中断0服务程序 使用了中断0
** 参数：	  无参数
** 返回值：    无
****************************************************************/
void EXTI0_IRQHandler(void)
{ 
	  EXTI->PR=1<<0;  //清除LINE0上的中断标志位

	if(SPEED_R==0)
	  {
	    if(txr>2)
		{
		  txr=0;
		  CodedDisk++;
	      MP++;
		}
	  	else txr++; 
	  }
	 
}

#endif
/***************************************************************
** 功能：     外部中断初始化
** 参数：	  无参数
** 返回值：    无
****************************************************************/
void EXTIX_Init(void)
{

	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	RCC->APB2ENR|=1<<3;    //使能PORTB时钟
	RCC->APB2ENR|=1<<6;    //使能PORTE时钟

	GPIOB->CRL&=0XFFFFFFF0;  // PB0 设置成输入	  
	GPIOB->CRL|=0X00000008;
	GPIOB->ODR|=1<<0; 
	
	GPIOB->CRH&=0XFFFFF00F;  // PB9/PB10设置成输入	  
	GPIOB->CRH|=0X00000880;
	GPIOB->ODR|=3<<9;   

	GPIOE->CRL&=0XFFFFFFF0;  // PE0设置成输出	  
	GPIOE->CRL|=0X00000003; 				   
	GPIOE->ODR|=1<<0;	     // PE0上拉
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟


  //GPIOB9/10 中断线以及中断初始化配置   下降沿触发
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource9);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line9;	
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
		
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //中断分组 2
	  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//子优先级3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 
		
#if SPPED_INT_MODE

 //GPIOB10	  中断线以及中断初始化配置 下降沿触发 
    
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource10);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line10;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
		
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //中断分组 2
		NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//子优先级2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);
		
#else

//GPIOB0	  中断线以及中断初始化配置 下降沿触发 
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//子优先级1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);
		
#endif

// 	Ex_NVIC_Config(GPIO_B,9,FTIR); //下降沿触发
// 	Ex_NVIC_Config(GPIO_B,10,FTIR); //下降沿触发
// 	MY_NVIC_Init(2,2,EXTI9_5_IRQHandler,2);    //抢占2，子优先级2，组2	
// 	MY_NVIC_Init(2,3,EXTI15_10_IRQHandler,2);  //抢占2，子优先级3，组2  

	
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
// 	RCC->APB1ENR|=1<<1;         // TIM3时钟使能    
//  	TIM3->ARR=arr;              // 设定计数器自动重装值//刚好1ms    
// 	TIM3->PSC=psc;              // 预分频器7200,得到10Khz的计数时钟
// 	
// 	/*这两个东东要同时设置才可以使用中断*/
// 	TIM3->DIER|=1<<0;           // 允许更新中断				
// 	TIM3->DIER|=1<<6;           // 允许触发中断
// 		  							    
// //	TIM3->CR1|=0x01;            // 使能定时器3
// //  	MY_NVIC_Init(1,3,TIM3_IRQHandler,3);//抢占1，子优先级3，组3		

  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //中断分组 2
	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM3, DISABLE);  //使能TIMx		ENABLE
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
