#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "key.h"
#include "flash.h"

const u8 table[] = "512K FLASH TEST";
#define size sizeof(table)
#define FLASH_SAVE_ADDR (uint32_t)0x08070000	

int main(void)
{
	u8 key;
	u8 i;
	u8 temp[size];
	SystemInit();			//系统初始化 可不写 在汇编启动文件中 main函数之前已执行
	delay_init();			//延时初始化
	uart_init(115200);		//串口1初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//中断分组2 两位抢占 两位响应
	LED_Init();
	LCD_Init();
	KEY_Init();
	POINT_COLOR=RED;			
	LCD_ShowString(30,50,200,16,16,"WarShip STM32");	
	LCD_ShowString(30,70,200,16,16,"Weihai College");	
	LCD_ShowString(30,90,200,16,16,"512K FLASH TEST");
	LCD_ShowString(30,110,200,16,16,"2016/09/28"); 
	LCD_ShowString(30,130,200,16,16,"KEY1:Write  KEY0:Read");
	while(1)
	{
		key = KEY_Scan(0);	
		switch(key)
		{
			case KEY0_PRES:				//按下KEY0 读取数据
				LCD_ShowString(30,170,200,16,16,"Start Read FLASH.... ");
				Flash_Read(FLASH_SAVE_ADDR,(u16*)temp,size);
				LCD_ShowString(30,170,200,16,16,"The Data Readed Is:  ");
				LCD_ShowString(30,190,200,16,16,temp);
				break;
			case KEY1_PRES:				//按下KEY1 写入数据
				LCD_Fill(0,170,239,319,WHITE);    
				LCD_ShowString(30,170,200,16,16,"Start Write FLASH....");
				Flash_Write(FLASH_SAVE_ADDR,(u16*)table,size);
				LCD_ShowString(30,170,200,16,16,"FLASH Write Finished!");
				break;
			case KEY2_PRES:				//按下KEY2 全片擦除 包括下载的程序
				FLASH_Unlock();
				FLASH_EraseAllPages();
				FLASH_Lock();
				break;
		}
		i++;
		if(i >= 40)
		{
			i = 0;
			LED1 = !LED1;
		}
		delay_ms(10);
	}	
}

