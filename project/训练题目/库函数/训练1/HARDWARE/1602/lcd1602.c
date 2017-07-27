#include "stm32f10x.h"
#include "lcd1602.h"
#include"delay.h"

/********************************************************************* 
*��    �ƣ�LCD_Writecmd()  
*��    �ܣ�дָ�LCD1602,ָ������ռһ���ֽ� 
*��ڲ�����cmd:��д���ָ�� 
*���ڲ�������  
*********************************************************************/ 
void LCD_Writecmd(u8 cmd) 
{  
	LCD_RS_0;    //��ͬһ���Ĵ���������д�룬�м���ʱһ��  
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
*��    �ƣ�LCD_Writedata()  
*��    �ܣ�дһ�ֽ����ݵ�LCD1602  
*��ڲ�����dat���޷����ֽ����ͣ�0~255  ��������ASCII���ַ� 
*���ڲ�������  
*********************************************************************/ 
void LCD_Writedata(u8 dat) 
{
	LCD_RS_1;
	delay_us(1);  
	LCD_RW_0;  
	delay_us(1);

	DATAOUT = dat;
	delay_ms(1);   
	LCD_EN_1;  //������     
	delay_ms(1); //����Ҫ����ʱ�������ԣ���ʱ300us���ϲſ���  
	LCD_EN_0;       //�½��أ���ʼд����Ч���� 
}

void LcdInit(void) 
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOC, ENABLE);//�����˿�D��ʱ��  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //PD0~PD7 ��©�����������©�����,������������������  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  
	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	   
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2);  //PC.0~2 ���������������©�����,������������������  
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


