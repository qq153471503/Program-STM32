#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "adc.h"
#include "touch.h"
#include "ili93xx.h"
#include "gui.h"
#include "wm.h"

int main(void)
{
	SystemInit();			//系统初始化 可不写 在汇编启动文件中 main函数之前已执行
	delay_init();			//延时初始化
	uart_init(115200);		//串口1初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//中断分组2 两位抢占 两位响应
	
	LED_Init();
	KEY_Init();
	Adc_Init();
	TFTLCD_Init();
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC,ENABLE);
	WM_SetCreateFlags(WM_CF_MEMDEV);
	GUI_Init();
	GUI_SetFont(&GUI_Font32B_ASCII);
	GUI_SetBkColor(GUI_BLACK);
	GUI_SetColor(GUI_YELLOW);
	GUI_Clear();
	GUI_DispStringAt("Hello World !", 0, 0);
	while(1)
	{
		
	}
}

