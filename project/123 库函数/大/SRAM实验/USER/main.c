#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "sram.h" 	  

int main(void)
{	 
	u8 key;		 
 	u8 i;
	u8 data[10] = "1234567890";
	u8 buf[10];

	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	KEY_Init();					//初始化按键
	LCD_Init();			   		//初始化LCD   
 	SRAM_FSMC_Init();			//初始化外部SRAM  
 	LED_Init();		  			//初始化与LED连接的硬件接口
 	POINT_COLOR=RED;			//设置字体为红色 
	LCD_ShowString(30,50,200,16,16,"WarShip STM32");	
	LCD_ShowString(30,70,200,16,16,"SRAM TEST");	
	LCD_ShowString(30,90,200,16,16,"KEY0: write");
	LCD_ShowString(30,110,200,16,16,"KEY1: read");
 	POINT_COLOR=BLUE;//设置字体为蓝色 
	 	 
  	while(1)
	{
		key = KEY_Scan(0);
		
		switch(key)
		{
			case KEY0_PRES:
			{
				SRAM_FSMC_Write(&data[0],0,10);
				LCD_ShowString(30,140,200,16,16,"Write Finished !");
			}break;
			case KEY1_PRES:
			{
				SRAM_FSMC_Read(&buf[0],0,10);
				LCD_ShowString(30,140,200,16,16,"Read  Finished !");
				LCD_ShowString(30,160,200,16,16,"data is:");
				LCD_ShowString(30,180,200,16,16,buf);
			}break;
			default :
				break;
		}
		delay_ms(10);
		i++;
		if(i==20)//DS0闪烁.
		{
			i=0;
			LED0=!LED0;
 		}
		LED0 = !LED0;
		delay_ms(500);
	}	      
}





