#ifndef _LCD1602_H_
#define _LCD1602_H_

#include "sys.h"

#define lcden0 GPIO_ResetBits(GPIOC,GPIO_Pin_2);
#define lcden1 GPIO_SetBits(GPIOC,GPIO_Pin_2);
#define lcdrw0 GPIO_ResetBits(GPIOC,GPIO_Pin_1);
#define lcdrw1 GPIO_SetBits(GPIOC,GPIO_Pin_1);
#define lcdrs0 GPIO_ResetBits(GPIOC,GPIO_Pin_0);
#define lcdrs1 GPIO_SetBits(GPIOC,GPIO_Pin_0);

#define Data GPIOD -> ODR	 /*���������ݿں궨��Ϊ Data*/

void LcdWriteCmd(u8 cmd);		  //д�����
void LcdWriteData(u8 dat);		  //д���ݺ���
void LcdIoInit(void);			  //I/O��ʼ������
void LcdInit(void);				  //lcd��ʼ������
void display(u8 addr,u8 dat);	  //�Զ���λ��д���ݺ���

#endif
