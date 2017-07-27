#include "led.h"
#include "delay.h"
#include "sys.h"
#include "key.h"
#include "usart.h"
#include "lcd.h"
#include "touch.h"



void Load_Drow_Dialog(void)
{
	LCD_Clear(WHITE);//����   
 	POINT_COLOR=BLUE;//��������Ϊ��ɫ 
	LCD_ShowString(216,0,"RST");//��ʾ��������
  	POINT_COLOR=RED;//���û�����ɫ 
}

 int main(void)
 {
	u8 i=0;	
	SystemInit();
	delay_init();	     //��ʱ��ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
 	uart_init(115200);
 	LED_Init();
 	KEY_Init();
	LCD_Init();

	POINT_COLOR=RED;//��������Ϊ��ɫ	   
	LCD_ShowString(60,50,"Mini STM32");	
	LCD_ShowString(60,70,"TOUCH TEST");	
	LCD_ShowString(60,90,"ATOM@ALIENTEK");
	LCD_ShowString(60,110,"2011/1/1");	
	LCD_ShowString(60,130,"Press KEY0 to Adjust");	
	Touch_Init();
	delay_ms(1500);
	Load_Drow_Dialog();	  
	while(1)
	{	
		touch_Scan();
		if(Pen_Point.Key_Sta==Key_Down)//������������
		{
			do
			{
				Convert_Pos();
				Pen_Point.Key_Sta=Key_Up;
				if(Pen_Point.X0>216&&Pen_Point.Y0<16)Load_Drow_Dialog();//���
				else 
				{
					Draw_Big_Point(Pen_Point.X0,Pen_Point.Y0);//��ͼ	    
					//GPIOC->ODR|=1<<1;    //PC1 ����	   
				}
			}while(PEN==0);//���PENһֱ��Ч,��һֱִ��
		}else delay_ms(10);

		i++;
		if(i==20)
		{
			i=0;
			LED0=!LED0;
		}
	};
 }

