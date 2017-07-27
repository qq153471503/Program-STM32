#include"stm32f10x.h"
#include"led.h"

void LedInit(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
	 	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOD, ENABLE);	 //使能PA端口时钟
		
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //LED0-->PA.8 端口配置
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_Init(GPIOA, &GPIO_InitStructure);
	 GPIO_SetBits(GPIOA,GPIO_Pin_8);						 //PA.8 输出高
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;     //设置I/O端口
	GPIO_Init(GPIOD, &GPIO_InitStructure);    //执行上面三句
	GPIO_SetBits(GPIOD,GPIO_Pin_2); 						 //PD.2 输出高 
}


