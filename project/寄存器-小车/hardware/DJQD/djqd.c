
#include "djqd.h"

/***************************************************************
** 功能：     电机驱动端口初始化
** 参数：	  无参数
** 返回值：    无
****************************************************************/
void DJ_Init(void)
{
	GPIOE->CRH&=0XFF000000; 
	GPIOE->CRH|=0X00333333;   //PE8/PE9/PE10/PE11/PE12/PE13推挽输出  	 
    GPIOE->ODR|=0X0000;

	LEFT111=1;
	LEFT112=1;
	RIGHT112=1;
	RIGHT111=1;

}

/***************************************************************
** 功能：     停车
** 参数：	  无
** 返回值：    无	  
****************************************************************/
void STOP(void)
{
	u16 st;
	for(st=0;st<10;st++)
	{
	  TIM1->CCR1=st*10;
      TIM1->CCR3=st*10;
 
	}

      LEFT111=0;LEFT112=0;
      RIGHT112=0;RIGHT111=0;

  
  

}


/***************************************************************
** 功能：     PWM初始化
** 参数：	  frequency：自动重装值
**            psc：时钟预分频数
** 返回值：    无	  
****************************************************************/
void PWM_Init(void)
{		 					 
//	u32 OCLK=72000000;		 					 
    //此部分需手动修改IO口设置
	RCC->APB2ENR|=1<<11; 	//TIM1时钟使能    
	GPIOE->CRH&=0XFF0FFFF0;	//PE8、PE13输出
	GPIOE->CRH|=0X00B0000B;	//复用功能输出 	  	 
	      
	AFIO->MAPR&=0XFFFFFF3F; //清除MAPR的[7:6]
	AFIO->MAPR|=1<<7;      //完全重映像,TIM1_CH1N->PE8
	AFIO->MAPR|=1<<6;      //完全重映像,TIM1_CH3->PE13

//	TIM1->ARR=OCLK/frequency;//设定计数器自动重装值 
//	TIM1->PSC=psc;			//预分频器不分频

	TIM1->ARR=100;        //设定计数器自动重装值  占空比比例0~100%
	TIM1->PSC=9;	      //10分频 //72KHZ PWM频率输出
	
	TIM1->CCMR2|=6<<4;  	//CH3 PWM2模式		 
	TIM1->CCMR2|=1<<3; 	    //CH2预装载使能	  

	TIM1->CCMR1|=6<<4;  	//CH1 PWM2模式		 
	TIM1->CCMR1|=1<<3; 	    //CH1预装载使能	 
	
	TIM1->CR1|=1<<7;     //ARPE使能自动重装载预装载允许位
	TIM1->CR1|=1<<4; 	   //向下计数模式

	TIM1->CCER|=3<<8;   //OC3 输出使能
	TIM1->CCER|=3<<2;   //OC1N 输出使能

// 	TIM1->BDTR |=1<<15;  //开启OC和OCN输出

	TIM1->EGR |= 1<<0; //初始化所有的寄存器
	TIM1->CR1|=1<<0;    //使能定时器1										  
}


/***************************************************************
** 功能：     电机控制函数
** 参数：	  L_Spend：电机左轮速度
**            R_Spend：电机右轮速度
** 返回值：    无	  
****************************************************************/
void Control(int L_Spend,int R_Spend)
{
		STOP();
	if(L_Spend>=0)
	{	
		if(L_Spend>100)L_Spend=100;if(L_Spend<5)L_Spend=5;		//限制速度参数
		LEFT111=0;LEFT112=1;TIM1->CCR1=L_Spend;                  //左轮正转计算公式
	}
	else
	{	
		if(L_Spend<-80)L_Spend=-80;if(L_Spend>-5)L_Spend=-5;     //限制速度参数
		LEFT111=1;LEFT112=0;TIM1->CCR1=-L_Spend;				 //左轮反转计算公式
	}
	if(R_Spend>=0)
	{	
		if(R_Spend>100)R_Spend=100;if(R_Spend<5)R_Spend=5;		//限制速度参数
		RIGHT112=0;RIGHT111=1;TIM1->CCR3=R_Spend;				//右轮正转计算公式
	}
	else
	{	
		if(R_Spend<-80)R_Spend=-80;if(R_Spend>-5)R_Spend=-5;		//限制速度参数		
		RIGHT112=1;RIGHT111=0;TIM1->CCR3=-R_Spend;					//右轮反转计算公式
	}

	TIM1->BDTR |=1<<15;  //开启OC和OCN输出
}
