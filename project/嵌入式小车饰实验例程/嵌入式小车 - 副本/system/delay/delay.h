#ifndef __DELAY_H
#define __DELAY_H 			   
#include <stm32f10x_lib.h>
//STM32������
//ʹ��SysTick����ͨ����ģʽ���ӳٽ��й���
//����delay_us,delay_ms
//V1.2
void DelayInit(u8 SYSCLK);
void delayms(u16 nms);
void delayus(u32 nus);

#endif





























