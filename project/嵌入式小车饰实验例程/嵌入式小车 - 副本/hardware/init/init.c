#include"sys.h"
#include"led.h"
#include"timer.h"
#include"track.h"

void Init(void)
{
	RCC->APB2ENR|=1<<2;    //使能PORTA时钟
	RCC->APB2ENR|=1<<3;    //使能PORTB时钟
	RCC->APB2ENR|=1<<4;    //使能PORTC时钟
	RCC->APB2ENR|=1<<5;    //使能PORTD时钟	
    RCC->APB2ENR|=1<<6;    //使能PORTE时钟
	
    RCC->APB2ENR|=1<<0;       //开启辅助时钟
    AFIO->MAPR&=0XF8FFFFFF;   //清除MAPR的［26：24］
    AFIO->MAPR|=0X04000000;   //关闭JTAG
	
	LedInit();
	XjInit();
	PwmInit(900,0);		
}
