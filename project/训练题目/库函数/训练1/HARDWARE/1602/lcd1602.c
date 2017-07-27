#include "stm32f10x.h"
#include "lcd1602.h"
#include"delay.h"

/********************************************************************* 
*名    称：LCD_Writecmd()  
*功    能：写指令到LCD1602,指令数据占一个字节 
*入口参数：cmd:待写入的指令 
*出口参数：无  
*********************************************************************/ 
void LCD_Writecmd(u8 cmd) 
{  
	LCD_RS_0;    //对同一个寄存器的两次写入，中间延时一会  
	delay_us(1);  
	LCD_RW_0;  
	delay_us(1);
	DATAOUT = cmd;
	delay_ms(1);    
	LCD_EN_1;     
	delay_ms(1);
	LCD_EN_0; 
}

/********************************************************************* 
*名    称：LCD_Writedata()  
*功    能：写一字节数据到LCD1602  
*入口参数：dat：无符号字节类型，0~255  包括各个ASCII码字符 
*出口参数：无  
*********************************************************************/ 
void LCD_Writedata(u8 dat) 
{
	LCD_RS_1;
	delay_us(1);  
	LCD_RW_0;  
	delay_us(1);

	DATAOUT = dat;
	delay_ms(1);   
	LCD_EN_1;  //先拉高     
	delay_ms(1); //很重要的延时，经调试，延时300us以上才可以  
	LCD_EN_0;       //下降沿，开始写入有效数据 
}

void LcdInit(void) 
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOC, ENABLE);//开启端口D的时钟  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //PD0~PD7 开漏输出，不能有漏极输出,除非外有有上拉电阻  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  
	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	   
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2);  //PC.0~2 推挽输出，不能有漏极输出,除非外有有上拉电阻  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;   
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	delay_ms(100);
	
		LCD_EN_0;
		LCD_RW_0;
		LCD_RS_0; 

	LCD_Writecmd(0x38);
	LCD_Writecmd(0x0C);
	LCD_Writecmd(0x06);
	LCD_Writecmd(0x01); 
}  
void LcdWriteChar(u8 cmd,u8 dat)
{
	LCD_Writecmd(cmd);
	LCD_Writedata(dat);
}


