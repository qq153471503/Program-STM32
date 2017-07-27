#include"lcd1602.h"
#include "delay.h"

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
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);	
}
/* LCD1602��ʼ������ */
void LcdInit(void)
{
	 lcden0;
	delay_us(10);
	 lcdrs0;
	delay_us(10);
	 lcdrw0;
	delay_us(10);

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

