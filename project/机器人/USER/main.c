/*####################################################*/
		#include "sys.h"
		#include "delay.h"
		#include "usart.h"
		#include "servo.h"
		#include "led.h"
		#include "key.h"
		#include "system.h"
/*####################################################*/

//舵机初始化归位函数
void Servo_Homing(void);
//机器人向前翻转函数
void qian_fan(void);

static unsigned int t=0 ;
static unsigned int k=0;
	
int main(void)
{
//	RCC_Config();				//使用HSI 系统初始化
	SystemInit();				//使用HSE 系统初始化
	delay_init();				//延时函数初始化
	LED_Init();					//LED灯初始化
	KEY_Init();					//按键初始化
	Servo_Left_PWM(2000,719);	//定时器2初始化	1k的计数频率 计数到2000是20ms   78/(719+1) = 1k
	Servo_Right_PWM(2000,719);	//定时器3初始化	1k的计数频率 计数到2000是20ms   78/(719+1) = 1k
	Servo_Homing();				//舵机全部归位函数 机器人立正姿态
	
	while(1)
	{
		qian_fan();
	}
	
}

/**
  * @brief  所有舵机初始化 机器人立正姿态
  * @param  void
  * @retval None
  * @instructions  静态内联函数，在其他文件中无法调用
  */
static __inline void Servo_Homing(void)
{	
	TIM_SetCompare1(TIM2,160);		//左腰
	TIM_SetCompare2(TIM2,150);		//左中
	TIM_SetCompare3(TIM2,155);		//左脚
	
	TIM_SetCompare1(TIM3,165);		//右yao
	TIM_SetCompare2(TIM3,145);		//右zhong
	TIM_SetCompare3(TIM3,150);		//右jiao
	
	//保持立正姿势后,延时一秒
	delay_ms(1000);
}


/**
  * @brief  实现机器人前翻滚
  * @param  void
  * @retval None
  * @instructions  静态内联函数，在其他文件中无法调用
  */
static __inline void qian_fan(void)
{
	for(t=150,k=145; t>=105,k<=190; t--,k++)
	{
		TIM_SetCompare2(TIM2,t);		//左中
		TIM_SetCompare2(TIM3,k);		//右zhong
		delay_ms(15);
	}
	
	for(t=155,k=155; t>=115,k<=195; t--,k++)
	{
		TIM_SetCompare3(TIM2,t);		//左脚		
		TIM_SetCompare3(TIM3,k);		//右jiao
		delay_ms(20);
	}
	
	for(t=105,k=190; t>=55,k<=240; t--,k++)
	{
		TIM_SetCompare2(TIM2,t);		//左中
		TIM_SetCompare2(TIM3,k);		//右zhong
		delay_ms(7);
	}
	
	for(t=160,k=165; t<=240,k>=80; t++,k--)
	{
		TIM_SetCompare1(TIM2,t);		//左腰
		TIM_SetCompare1(TIM3,k);		//右yao
		delay_ms(7);
	}
	
	for(t=115,k=195; t<=155,k>=155; t++,k--)
	{
		TIM_SetCompare3(TIM2,t);		//左脚		
		TIM_SetCompare3(TIM3,k);		//右jiao
		delay_ms(7);
	}
	
/**
 * 回翻开始
 ********/
	
	for(t=240,k=80; t>=120,k<=200; t--,k++)
	{
		TIM_SetCompare1(TIM2,t);		//左腰
		TIM_SetCompare1(TIM3,k);		//右yao
		delay_ms(12);
	}
	
	
	for(t=55,k=240; t<=245,k>=50; t++,k--)
	{
		TIM_SetCompare2(TIM2,t);		//左中
		TIM_SetCompare2(TIM3,k);		//右zhong
		delay_ms(7);
	}

	for(t=120,k=200; t>=85,k<=235; t--,k++)
	{
		TIM_SetCompare1(TIM2,t);		//左腰
		TIM_SetCompare1(TIM3,k);		//右yao
		delay_ms(10);
	}

/**
 * 起身
 *******/	
	for(t=245,k=50; t>=145,k<=150; t--,k++)
	{
		TIM_SetCompare2(TIM2,t);		//左中
		TIM_SetCompare2(TIM3,k);		//右zhong	
		delay_ms(10);
	}
	
	for(t=90,k=235; t<=160,k>=165; t++,k--)
	{
		TIM_SetCompare1(TIM2,t);		//左腰
		TIM_SetCompare1(TIM3,k);		//右yao
		delay_ms(14);
	}
}



