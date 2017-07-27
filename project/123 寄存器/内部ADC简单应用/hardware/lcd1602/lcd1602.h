#ifndef _LCD1602_H_
#define _LCD1602_H_

#include"sys.h"
#include"delay.h"

//ʹ�� ��0
#define lcden0 GPIOE -> BRR = 0x0004			//lcdʹ�� PE2
//ʹ�� ��1
#define lcden1 GPIOE -> BSRR = 0X0004

#define lcdrs0 GPIOE -> BRR = 0X0001			// PE1
#define lcdrs1 GPIOE -> BSRR = 0X0001

#define lcdrw0 GPIOE -> BRR = 0X0002			//PE0
#define lcdrw1 GPIOE -> BSRR = 0X0002

#define Data GPIOD -> ODR	 /*���������ݿں궨��Ϊ Data*/

void LcdWriteCmd(u8 cmd);		  //д�����
void LcdWriteData(u8 dat);		  //д���ݺ���
void LcdIoInit(void);			  //I/O��ʼ������
void LcdInit(void);				  //lcd��ʼ������
void display(u8 addr,u8 dat);	  //�Զ���λ��д���ݺ���

#endif
