#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "key.h"
#include "can.h"

//mode设置
// CAN_Mode_Normal             ((uint8_t)0x00)  /*!< normal mode */
// CAN_Mode_LoopBack           ((uint8_t)0x01)  /*!< loopback mode */
// CAN_Mode_Silent             ((uint8_t)0x02)  /*!< silent mode */
// CAN_Mode_Silent_LoopBack    ((uint8_t)0x03)  /*!< loopback combined with silent mode */

int main(void)
{
	u8 mode = CAN_Mode_LoopBack; //初始化为回环模式
	u8 count = 0;
	u8 key;
	u8 len = 0;
	u8 i,t = 0;
	u8 res;
	u8 CanBuf[8];
	
	SystemInit();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	delay_init();
	uart_init(115200);
	LED_Init();
	LCD_Init();
	KEY_Init();
	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS1_9tq,CAN_BS2_8tq,4,mode);
	
 	POINT_COLOR=RED;//设置字体为红色 
	LCD_ShowString(60,50,200,16,16,"WarShip STM32");	
	LCD_ShowString(60,70,200,16,16,"CAN TEST");	
	LCD_ShowString(60,90,200,16,16,"WeiHai College");
	LCD_ShowString(60,110,200,16,16,"2016/9/19");
	LCD_ShowString(60,130,200,16,16,"LoopBack Mode");	 
	LCD_ShowString(60,150,200,16,16,"KEY0:Send WK_UP:Mode");//显示提示信息		
	POINT_COLOR=BLUE;//设置字体为蓝色	  
	LCD_ShowString(60,170,200,16,16,"Count:");			//显示当前计数值	
	LCD_ShowString(60,190,200,16,16,"Send Data:");		//提示发送的数据	
	LCD_ShowString(60,250,200,16,16,"Receive Data:");	//提示接收到的数据		
	
	while(1)
	{
		key = KEY_Scan(0);
		if(key == KEY0_PRES)
		{
			for(i=0; i<8; i++)
			{
				CanBuf[i] = count+i;	//填充发送缓冲区
				if(i<4)
					LCD_ShowxNum(60+i*32,210,CanBuf[i],3,16,0x80);
				else 
					LCD_ShowxNum(60+(i-4)*32,230,CanBuf[i],3,16,0x80);
			}
			res = CAN_Send_Msg(CanBuf,8);
			if(res)
				LCD_ShowString(60+80,190,200,16,16,"Failed");		//提示发送失败
			else 
				LCD_ShowString(60+80,190,200,16,16,"OK    ");	 		//提示发送成功	
		}else if(key == WKUP_PRES)
		{
			mode = !mode;
			CAN_Mode_Init(CAN_SJW_1tq,CAN_BS1_9tq,CAN_BS2_8tq,4,mode);
			POINT_COLOR=RED;//设置字体为红色 
			if(mode==0)//普通模式，需要2个开发板
			{
				LCD_ShowString(60,130,200,16,16,"Nnormal Mode ");	    
			}else //回环模式,一个开发板就可以测试了.
			{
 				LCD_ShowString(60,130,200,16,16,"LoopBack Mode");
			}
 			POINT_COLOR=BLUE;//设置字体为蓝色 
		}
		
		len = CAN_Receive_Msg(CanBuf);
		if(len)
		{
			LCD_Fill(60,270,130,310,WHITE);//清除之前的显示
 			for(i=0;i<len;i++)
			{									    
				if(i<4)
					LCD_ShowxNum(60+i*32,270,CanBuf[i],3,16,0X80);	//显示数据
				else 
					LCD_ShowxNum(60+(i-4)*32,290,CanBuf[i],3,16,0X80);	//显示数据
 			}
		}
		t++;
		if(t == 20)
		{
			count++;
			t = 0;
			LED0 = !LED0;
			LCD_ShowxNum(60+48,170,count,3,16,0X80);	//显示数据
		}
		delay_ms(10);
	}
}

