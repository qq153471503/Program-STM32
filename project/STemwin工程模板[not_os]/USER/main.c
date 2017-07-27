#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "ILI93XX.h"
#include "usart.h"	  
#include "sram.h"
#include "malloc.h"
#include "string.h"
#include "usmart.h"	
#include "GUI.H"

 int main(void)
 {	 
	delay_init();	    	 //延时函数初始化	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
 	LED_Init();		  			//初始化与LED连接的硬件接口
	KEY_Init();					//初始化按键
	TFTLCD_Init();			   		//初始化LCD   
 	FSMC_SRAM_Init();			//初始化外部SRAM  
	my_mem_init(SRAMIN);		//初始化内部内存池
	my_mem_init(SRAMEX);		//初始化外部内存池

	RCC_AHBPeriphClockCmd(RCC_AHBENR_CRCEN, ENABLE);	//打开CRC时钟
	GUI_Init();
	GUI_SetBkColor(GUI_BLACK);
	GUI_SetColor(GUI_YELLOW);
	GUI_SetFont(&GUI_Font32_ASCII);
	GUI_Clear();
	GUI_DispStringAt("Hello STemwin!",10, 10);
  	while(1)
	{	
		
	}
}
 



