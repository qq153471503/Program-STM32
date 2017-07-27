#include"stm32f10x.h"
#include"led.h"
#include"timer.h"

int main(void)
{
	SystemInit();/*系统时钟初始化为72MHz*/
	LedInit();
	TimerInit(4999,7199);  /*计数到5000 为0.5秒*/
	
	
	while(1);
}

