#include"pwm.h"

void PwmInit(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;		//设置通道的语句
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);//开启端口x的时钟
	  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;     //设置I/O端口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //设置输出/输出模式
	GPIO_Init(GPIOA, &GPIO_InitStructure);    //执行上面三句

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;     //设置I/O端口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //设置输出/输出模式  复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    //执行上面

	//定时器TIMx初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_Trigger,ENABLE); 
 //定时器x 允许更新中断 允许触发中断 使能TIMx

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;

//选择定时器模式: TIM_OCMode_PWM1;  TIM_OCMode_PWM2;

	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高TIM_OCPolarity_Low;//低
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIMx在CCRx(通道x)上的预装载寄存器
	TIM_ARRPreloadConfig(TIM3, ENABLE); //使能TIMx在ARR上的预装载寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIMx
}



	
