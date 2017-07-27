#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "includes.h"

int main(void)
{
	delay_init();       	//延时初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //中断分组配置
	uart_init(115200);    	//串口波特率设置

}

