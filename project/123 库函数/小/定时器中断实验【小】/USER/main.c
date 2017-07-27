#include "led.h"
#include "delay.h"
#include "sys.h"
#include "timer.h"
	

 int main(void)
 {
   	SystemInit();
	delay_init(72);	     //延时初始化
 	LedInit();		   //LED端口初始化
	TimerInit(10000,7199);//10Khz的计数频率，计数到5000为500ms 
	while(1)
	{
		led0 =!led0;		//红led灯
		delay_ms(200);		   
	}
 }

