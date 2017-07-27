#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
 
int main(void)
{		
	u8 i=0x00;
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart2_init(115200);	 				//串口初始化为115200
	LED_Init();			     			//LED端口初始化
	KEY_Init();          				//初始化与按键连接的硬件接口
	while(1)
	{
		USART_SendData(USART2,i++);
		delay_ms(1000);
	}
}

