#include "stm32f10x.h"
#include "djqd.h"
#include "canp_hostcom.h"
#include "test.h"

u8 R_Speed =0;
u8 L_Speed =0;

u8 flag_djzt =0;  // 电机状态标志位
u8 timer6_num =0; // 定时器计数


/***************************************************************
** 功能：     电机驱动端口初始化
** 参数：	  无参数
** 返回值：    无
****************************************************************/
void DJ_Init(void)
{
	RCC->APB2ENR|=1<<6;    //使能PORTE时钟
	GPIOE->CRH&=0XF00F0000; 
	GPIOE->CRH|=0X03303333;   //PE8/PE9/PE10/PE11/PE13/PE14推挽输出  	 
  GPIOE->ODR|=0X0000;

	LEFT_NSLEEP = 1;
	REFT_NSLEEP = 1;
	
	REFT1_PWM =1;
	REFT2_PWM =1;
	
	LEFT1_PWM =0;
	LEFT2_PWM =0;
	
	
}

/***************************************************************
** 功能：     停车
** 参数：	  无
** 返回值：    无	  
****************************************************************/
void STOP(void)
{
		u16 st;

	TIM_Cmd(TIM2, DISABLE);
	
//	for(st=0;st<10;st++)
//	{
//	  TIM1->CCR1=st*10;
//    TIM1->CCR2=st*10;
//	  TIM1->CCR3=st*10;
//    TIM1->CCR4=st*10;
//	}
	  TIM1->CCR1=100;
    TIM1->CCR2=100;
	  TIM1->CCR3=100;
    TIM1->CCR4=100;	
	
//	  TIM1->BDTR &=~(1<<15);  //开启OC和OCN输出
	
	  LEFT_NSLEEP = 1;
	  REFT_NSLEEP = 1;
	
	  Send_UpMotor(0 , 0);  // 上传电机速度
	
}

/***************************************************************
** 功能：     PWM初始化
** 参数：	  frequency：自动重装值
**            psc：时钟预分频数
** 返回值：    无	  
****************************************************************/
void PWM_Init(void)
{	
	
	#if 1

//	u32 OCLK=72000000;		 					 
    //此部分需手动修改IO口设置
	RCC->APB2ENR|=1<<11; 	//TIM1时钟使能  
  RCC->APB2ENR|=1<<6;    //使能PORTE时钟  
	RCC->APB2ENR|=1<<0;       //开启辅助时钟 
	
	GPIOE->CRH&=0XF00F0F0F;	//PE9/11/13/14输出
	GPIOE->CRH|=0X0BB0B0B0;	//复用功能输出 	  	 
	
   
	AFIO->MAPR&=0XFFFFFF3F; //清除MAPR的[7:6]
	AFIO->MAPR|=1<<7;      //完全重映像,TIM1_CH1->PE9  L1   TIM1_CH2->PE11 L2 
	AFIO->MAPR|=1<<6;      //完全重映像,TIM1_CH3->PE13 R1   TIM1_CH4->PE14 R2 

//	TIM1->ARR=OCLK/frequency;//设定计数器自动重装值 
//	TIM1->PSC=psc;			//预分频器不分频

	TIM1->ARR=100;        //设定计数器自动重装值  占空比比例0~100%
	TIM1->PSC=9;	      //10分频 //72KHZ PWM频率输出

	TIM1->CCMR1|=6<<4;  	//CH1 PWM1模式		 
	TIM1->CCMR1|=1<<3; 	  //CH1 预装载使能	
	
	TIM1->CCMR1|=6<<12;  	//CH2 PWM1模式		 
	TIM1->CCMR1|=1<<11; 	//CH2 预装载使能	
	
	TIM1->CCMR2|=6<<4;  	//CH3 PWM1模式		 
	TIM1->CCMR2|=1<<3; 	  //CH3 预装载使能	  

	TIM1->CCMR2|=6<<12;  	//CH4 PWM1模式		 
	TIM1->CCMR2|=1<<11; 	//CH4 预装载使能
	
	
	TIM1->CR1|=1<<7;     //ARPE使能自动重装载预装载允许位
	TIM1->CR1|=1<<4; 	   //向下计数模式

	TIM1->CCER|=1<<0;   //OC1 输出使能   高电平有效
	TIM1->CCER|=1<<4;   //OC2 输出使能
	TIM1->CCER|=1<<8;   //OC3 输出使能
	TIM1->CCER|=1<<12;  //OC4 输出使能
	


	TIM1->EGR |= 1<<0; //初始化所有的寄存器
	TIM1->CR1|=1<<0;    //使能定时器1		

	TIM1->CCR1= 50;
	TIM1->CCR2= 00;  
//	TIM1->BDTR |=1<<15;  //开启OC和OCN输出

#endif
	
}


/***************************************************************
** 功能：     电机控制函数
** 参数：	  L_Spend：电机左轮速度
**            R_Spend：电机右轮速度
** 返回值：    无	  
****************************************************************/
void Control(int L_Spend,int R_Spend)
{
	//	STOP();
	TIM_Cmd(TIM2, ENABLE);
	Send_UpMotor(L_Spend ,R_Spend);

  if(L_Spend>=0)
	{	
		if(L_Spend>100)L_Spend=100;if(L_Spend<5)L_Spend=5;		//限制速度参数
     LEFT_NSLEEP =1;  TIM1->CCR2= L_Spend; TIM1->CCR1= 0;
	}
	else 
	{
    if(L_Spend<-100)L_Spend= -100;if(L_Spend>-5)L_Spend= -5;     //限制速度参数
      LEFT_NSLEEP =1;  TIM1->CCR1= -L_Spend; TIM1->CCR2= 0; 
  }
	
	if(R_Spend>=0)
	{	
		if(R_Spend>100)R_Spend=100;if(R_Spend<5)R_Spend=5;		//限制速度参数
     REFT_NSLEEP =1;  TIM1->CCR3= R_Spend; TIM1->CCR4= 0;
	}
	else
	{	
		if(R_Spend<-100)R_Spend= -100;if(R_Spend>-5)R_Spend= -5;		//限制速度参数		
     REFT_NSLEEP =1;  TIM1->CCR4= -R_Spend; TIM1->CCR3= 0;
	}
	
	TIM1->BDTR |=1<<15;  //开启OC和OCN输出
		
}

void Go_Test( u8 sp ,u8 len)  // 以50的速度前进距离为50
{
    MP=0;G_Flag=1;	Stop_Flag=0;tempMP=0;			 					
		tempMP= len;  
		Car_Spend = sp;
	  Control(Car_Spend,Car_Spend);
}

void Back_Test(u8 sp ,u8 len )  // 以50的速度后退距离为50
{

	MP=0;B_Flag=1;	Stop_Flag=0;tempMP=0;
	tempMP= len;  
	Car_Spend = sp;
	Control(-Car_Spend,-Car_Spend);

}

void Left_Test( u8 sp)  // 以80的速度左转弯
{

	MP=0;L_Flag=1;	Stop_Flag=0;
	Car_Spend = sp;
	Host_Open_UpTrack( Up_Track_Speed );
	Control(-Car_Spend,Car_Spend);

}

void Right_Test(u8 sp)  // 以80的速度右转弯
{
   MP=0;R_Flag=1;	Stop_Flag=0;
	 Car_Spend = sp;
	 Host_Open_UpTrack( Up_Track_Speed );
	 Control(Car_Spend,-Car_Spend);

}

void Track_Test( u8 sp)  // 已50的速度循迹
{

		Car_Spend = sp;
		Track_Flag=1;MP=0; 					  
		Host_Open_UpTrack( Up_Track_Speed );  // 开启寻迹数据上传
		delay_ms( 100); // 等待一会寻迹数据上传
		TIM_Cmd(TIM2, ENABLE);

}

void Stop_Test(void)
{
		Track_Flag=0;MP=0;
		Stop_Flag=0;G_Flag=0;B_Flag=0;L_Flag=0;R_Flag=0; 
	  STOP();


}




