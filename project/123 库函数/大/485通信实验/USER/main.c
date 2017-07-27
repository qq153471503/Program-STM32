#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "rs485.h"
//ALIENTEK战舰STM32开发板实验24
//485 实验  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司  
 				 	
 int main(void)
 {	 
	u8 key;
	u8 i=0,t=0;
	u8 cnt=0;
	u8 rs485buf[5];
	delay_init();	    	 //延时函数初始化	  
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(9600);	 	//串口初始化为9600
 	LedInit();			     //LED端口初始化
	LCD_Init();			     //LCD初始化
	KeyInit();	 			 //KEY初始化
	RS485_Init(9600);	//初始化RS485

 	POINT_COLOR=RED;//设置字体为红色 
	LCD_ShowString(60,50,200,16,16,"WarShip STM32");	
	LCD_ShowString(60,70,200,16,16,"RS485 TEST");	
	LCD_ShowString(60,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(60,110,200,16,16,"2012/9/9");	
	LCD_ShowString(60,130,200,16,16,"KEY0:Send");	//显示提示信息	
 
 	POINT_COLOR=BLUE;//设置字体为蓝色	  
	LCD_ShowString(60,150,200,16,16,"Count:");			//显示当前计数值	
	LCD_ShowString(60,170,200,16,16,"Send Data:");		//提示发送的数据	
	LCD_ShowString(60,210,200,16,16,"Receive Data:");	//提示接收到的数据		
 									  
	while(1)
	{
		key=KeyScan(0);
		if(key==KEY_RIGHT)//KEY0按下,发送一次数据
		{
			for(i=0;i<5;i++)
			{
				rs485buf[i]=cnt+i;//填充发送缓冲区
				LCD_ShowxNum(60+i*32,190,rs485buf[i],3,16,0X80);	//显示数据
 			}
			RS485_Send_Data(rs485buf,5);//发送5个字节 									   
		}		 
		RS485_Receive_Data(rs485buf,&key);
		if(key)//接收到有数据
		{
			if(key>5)key=5;//最大是5个数据.
 			for(i=0;i<key;i++)LCD_ShowxNum(60+i*32,230,rs485buf[i],3,16,0X80);	//显示数据
 		}
		t++; 
		delay_ms(10);
		if(t==20)
		{
			led0=!led0;//提示系统正在运行	
			t=0;
			cnt++;
			LCD_ShowxNum(60+48,150,cnt,3,16,0X80);	//显示数据
		}		   
	}
}


