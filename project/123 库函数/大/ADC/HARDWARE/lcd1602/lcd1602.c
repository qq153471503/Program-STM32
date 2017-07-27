#include "lcd1602.h"
#include "delay.h"

void LcdWriteCmd(u8 cmd)
{
	lcdrw0;
	delay_us(100);
	lcdrs0;
	delay_us(100);
	
	LcdData = cmd;
	delay_ms(2);
	
	lcden1;
	delay_ms(2);
	lcden0;
	delay_ms(2);		
}
void LcdWriteData(u8 dat)
{
	lcdrw0;
	delay_us(100);
	lcdrs1;
	delay_us(100);
	
	LcdData = dat;
	delay_ms(2);
	
	lcden1;
	delay_ms(2);
	lcden0;
	delay_ms(2);		
}
void LcdInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
//	GPIO_SetBits(GPIOD,GPIO_Pin_All);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
//	GPIO_SetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2);
	
	
	lcden0;
	delay_us(10);
	lcdrs0;
	delay_us(10);
	lcdrw0;
	delay_us(10);
	
	LcdWriteCmd(0x38);
	LcdWriteCmd(0x0c);
	LcdWriteCmd(0x06);
	LcdWriteCmd(0x01);
}
void LcdAny(u8 cmd,u8 dat)
{
	LcdWriteCmd(cmd);
	LcdWriteData(dat);
}
