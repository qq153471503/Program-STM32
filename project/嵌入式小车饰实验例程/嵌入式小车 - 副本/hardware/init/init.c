#include"sys.h"
#include"led.h"
#include"timer.h"
#include"track.h"

void Init(void)
{
	RCC->APB2ENR|=1<<2;    //ʹ��PORTAʱ��
	RCC->APB2ENR|=1<<3;    //ʹ��PORTBʱ��
	RCC->APB2ENR|=1<<4;    //ʹ��PORTCʱ��
	RCC->APB2ENR|=1<<5;    //ʹ��PORTDʱ��	
    RCC->APB2ENR|=1<<6;    //ʹ��PORTEʱ��
	
    RCC->APB2ENR|=1<<0;       //��������ʱ��
    AFIO->MAPR&=0XF8FFFFFF;   //���MAPR�ģ�26��24��
    AFIO->MAPR|=0X04000000;   //�ر�JTAG
	
	LedInit();
	XjInit();
	PwmInit(900,0);		
}
