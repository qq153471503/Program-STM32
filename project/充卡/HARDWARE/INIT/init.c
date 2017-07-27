#include "stm32f10x.h"
#include "init.h"
#include "led.h"		 	 
#include "test.h"
#include "djqd.h"
#include "key.h"
#include "xj.h"
#include "csb.h"
#include "hw.h"
#include "hmc5883l.h"
#include "bh1750.h"
#include "exit.h"
#include "timer.h"
#include "power_check.h"
#include "syn7318.h"

/***************************************************************
** 功能：     硬件端口初始化  使能端口时钟
** 参数：	  无参数
** 返回值：   无
****************************************************************/
void YJ_INIT(void)
{
	RCC->APB2ENR|=1<<2;    //使能PORTA时钟
	RCC->APB2ENR|=1<<3;    //使能PORTB时钟
	RCC->APB2ENR|=1<<4;    //使能PORTC时钟
	RCC->APB2ENR|=1<<5;    //使能PORTD时钟	
    RCC->APB2ENR|=1<<6;    //使能PORTE时钟   	 

    RCC->APB2ENR|=1<<0;       //开启辅助时钟
    AFIO->MAPR&=0XF8FFFFFF;   //清除MAPR的［26：24］
  //  AFIO->MAPR|=0X04000000;   //关闭JTAG
	 
	 LED_Init();			 //LED硬件初始化
	#if 1
	 DJ_Init();				 //电机驱动初始化
	 PWM_Init();             //PWM初始化 
	 IIC_Init();	        //初始化IIC
 	 BH1750_Init();         //初始化光照
	 KEY_Init();			//按键初始化
//	 HC595_Init();		   //74HC595初始化
	 XJ_Init();				//循迹初始化
	 Timer3_Init(10,71);     // 1Mhz的计数频率，计数到10为10us  
	 Timer2_Init(1000,71);     // 1Mhz的计数频率，计数到500为100us
	 Timer4_Init(1000,71);     // 1Mhz的计数频率，计数到1000为1ms
	 TIM6_Int_Init(2000,71); // 2ms 检测CAN总线数据
   EXTIX_Init();         	// 外部中断初始化
	 HW_Init();				//红外发射初始化
	 SYN7318_Init();  // 语音识别测试初始化
	 
	 Electricity_Init();
	 #endif
}

