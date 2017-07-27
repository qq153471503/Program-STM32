#include"led.h"

void LedInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOD, ENABLE);//开启端口x的时钟  

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;     //设置I/O端口
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   //设置输入/输出模式的方式速度
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //设置输出/输出模式
	GPIO_Init(GPIOA, &GPIO_InitStructure);    //执行上面三句
	GPIO_SetBits(GPIOA,GPIO_Pin_8);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;     //设置I/O端口
	GPIO_Init(GPIOD, &GPIO_InitStructure);    //执行上面三句
	GPIO_SetBits(GPIOD,GPIO_Pin_2);
}


