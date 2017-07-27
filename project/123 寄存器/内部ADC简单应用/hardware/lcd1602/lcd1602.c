#include"lcd1602.h"

/*��LCD1602д����**/
void LcdWriteCmd(u8 cmd)
{
	lcdrs0;				  //д0  ѡ��д����
	delay_ms(1);
	lcdrw0;				  //д0 ѡ��д����
	delay_ms(1);

	Data = cmd;			 //дʱ��
	delay_ms(1);
	lcden1;
	delay_ms(1);
	lcden0;
}
/*��LCD1602д������*/
void LcdWriteData(u8 dat)
{
	lcdrs1;				//д1 ѡ��д����
	delay_ms(1);
	lcdrw0;				//д0  ѡ��д����
	delay_ms(1);

	Data = dat;			//дʱ��
	delay_ms(1);
	lcden1;
	delay_ms(1);
	lcden0;
}
/* I/O �� ��ʼ������*/
void LcdIoInit(void)
{
	RCC -> APB2ENR |= 1<<5;				  //ʹ��GPIODʱ��
	RCC -> APB2ENR |= 1<<6;				  //ʹ��GPIOEʱ��

	GPIOD -> CRL &= 0X00000000;
	GPIOD -> CRL |= 0X77777777;
	GPIOD -> ODR |= 0XFFFF;

	GPIOE -> CRL &= 0XFFFFF000;
	GPIOE -> CRL |= 0X00000777;
	GPIOE -> ODR |= 0X0007;
}
/* LCD1602��ʼ������ */
void LcdInit(void)
{
	 lcden0;
	 lcdrs0;
	 lcdrw0;

	LcdWriteCmd(0x38);	  //���� 16*2��ʾ��ʽ  5*7���� 8λ���ݽӿ�
	LcdWriteCmd(0x0c);	  //����ʾ ����ʾ���
	LcdWriteCmd(0x06);	  //дһ���ַ� ָ���Զ���һ
	LcdWriteCmd(0x01);	  //����
}

void display(u8 addr,u8 dat)
{
	LcdWriteCmd(addr);		//����д�����
	LcdWriteData(dat);		//����д���ݺ���
}

