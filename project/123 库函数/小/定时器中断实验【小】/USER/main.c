#include "led.h"
#include "delay.h"
#include "sys.h"
#include "timer.h"
	

 int main(void)
 {
   	SystemInit();
	delay_init(72);	     //��ʱ��ʼ��
 	LedInit();		   //LED�˿ڳ�ʼ��
	TimerInit(10000,7199);//10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms 
	while(1)
	{
		led0 =!led0;		//��led��
		delay_ms(200);		   
	}
 }

