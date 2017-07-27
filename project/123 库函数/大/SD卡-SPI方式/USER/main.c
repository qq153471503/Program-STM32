#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "key.h"
#include "mmc_sd.h"

u8 buf[512];//SD卡数据缓存区
u8 write_buf[512];
u32 sector_cnt;//扇区总数        7729152个扇区
u8 sta;

void Card_Info(void);

int main(void)
{
 	u32 sector_size;
	u16 t=0;	
	SystemInit();			//系统初始化 可不写 在汇编启动文件中 main函数之前已执行
	delay_init();			//延时初始化
	uart_init(115200);		//串口1初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//中断分组2 两位抢占 两位响应
	LCD_Init();
	LED_Init();
	KEY_Init();
	
	POINT_COLOR=RED;//设置字体为红色	
	LCD_ShowString(20,50,200,16,16,"Mini STM32");	
	LCD_ShowString(20,70,200,16,16,"SD Card TEST");	
	LCD_ShowString(20,90,100,16,16,"2016-12-22");	

	for(t=0; t<sizeof(write_buf); t++)			//填充要写入SD卡的数组缓冲区数据
	{
		write_buf[t] = t;
		LCD_ShowxNum(10,10,t,3,16,0);
	}
	
	while(SD_Initialize()!=0)//检测不到SD卡
	{
		LCD_ShowString(20,110,200,16,16,"SD Card Failed!");
		delay_ms(500);
		LCD_ShowString(20,110,200,16,16,"Please Check!      ");
		delay_ms(500);
		LED0=!LED0;//DS0闪烁
	}
    Card_Info();
	while(1)
	{
		if(!KEY0)
		{
			delay_ms(10);
			if(!KEY0)
			{
				if(!(SD_ReadDisk(buf,0,1)))//读取MBR扇区
				{	
					LCD_ShowString(20,185,200,16,16,"USART1 Sending Data...    ");
					printf("SECTOR 0 DATA:\n");
					for(sector_size=0;sector_size<512;sector_size++)printf("%x ",buf[sector_size]);//打印MBR扇区数据    	   
					printf("\nDATA ENDED\n");
					LCD_ShowString(20,185,200,16,16,"USART1 Send Data Over !!!    ");
				}
			}while(!KEY0);
		} 

		if(!KEY1)
		{
			delay_ms(10);
			if(!KEY1)
			{
				sta = SD_WriteDisk(write_buf,0,1);
				if(!sta)
					LCD_ShowString(20,185,200,16,16,"SD Card Write Successful !!!");
			}while(!KEY0);
		} 		

	}
}

void Card_Info(void)
{	
	sector_cnt = SD_GetSectorCount();
	LCD_ShowString(20,110,200,16,16,"SD Card check successful !!!");
	LCD_ShowString(20,130,200,16,16,"The Card type is ");
	LCD_ShowString(20,150,200,16,16,"The Card size is:      MB");
	LCD_ShowxNum(160,150,(sector_cnt*512/1024/1024),5,16,0);			// 扇区总数*每个扇区大小=总字节  总字节/1024/1024 = MB
}
