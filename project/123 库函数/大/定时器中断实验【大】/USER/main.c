#include"stm32f10x.h"
#include"led.h"
#include"timer.h"

int main(void)
{
	SystemInit();/*ϵͳʱ�ӳ�ʼ��Ϊ72MHz*/
	LedInit();
	TimerInit(4999,7199);  /*������5000 Ϊ0.5��*/
	
	
	while(1);
}

