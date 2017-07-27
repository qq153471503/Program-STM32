#include "stm32f10x.h"
#include "init.h"
#include "led.h"		 	 
#include "test.h"
#include "djqd.h"
#include "key.h"
#include "xj.h"
#include "csb.h"
#include "hw.h"
#include "hmc5883l.h"
#include "bh1750.h"
#include "exit.h"
#include "timer.h"
#include "power_check.h"
#include "syn7318.h"

/***************************************************************
** ���ܣ�     Ӳ���˿ڳ�ʼ��  ʹ�ܶ˿�ʱ��
** ������	  �޲���
** ����ֵ��   ��
****************************************************************/
void YJ_INIT(void)
{
	RCC->APB2ENR|=1<<2;    //ʹ��PORTAʱ��
	RCC->APB2ENR|=1<<3;    //ʹ��PORTBʱ��
	RCC->APB2ENR|=1<<4;    //ʹ��PORTCʱ��
	RCC->APB2ENR|=1<<5;    //ʹ��PORTDʱ��	
    RCC->APB2ENR|=1<<6;    //ʹ��PORTEʱ��   	 

    RCC->APB2ENR|=1<<0;       //��������ʱ��
    AFIO->MAPR&=0XF8FFFFFF;   //���MAPR�ģ�26��24��
  //  AFIO->MAPR|=0X04000000;   //�ر�JTAG
	 
	 LED_Init();			 //LEDӲ����ʼ��
	#if 1
	 DJ_Init();				 //���������ʼ��
	 PWM_Init();             //PWM��ʼ�� 
	 IIC_Init();	        //��ʼ��IIC
 	 BH1750_Init();         //��ʼ������
	 KEY_Init();			//������ʼ��
//	 HC595_Init();		   //74HC595��ʼ��
	 XJ_Init();				//ѭ����ʼ��
	 Timer3_Init(10,71);     // 1Mhz�ļ���Ƶ�ʣ�������10Ϊ10us  
	 Timer2_Init(1000,71);     // 1Mhz�ļ���Ƶ�ʣ�������500Ϊ100us
	 Timer4_Init(1000,71);     // 1Mhz�ļ���Ƶ�ʣ�������1000Ϊ1ms
	 TIM6_Int_Init(2000,71); // 2ms ���CAN��������
   EXTIX_Init();         	// �ⲿ�жϳ�ʼ��
	 HW_Init();				//���ⷢ���ʼ��
	 SYN7318_Init();  // ����ʶ����Գ�ʼ��
	 
	 Electricity_Init();
	 #endif
}

