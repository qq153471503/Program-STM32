#include "lcd12864.h"
#include "delay.h"
#include "sys.h"
#include "pic.h"


u8 table[12] = "����ְҵѧԺ";
u8 table1[4] = "D545";
u8 table2[16] = "ѧ��:20150203039";
u8 table3[12] = "������������";

void StartDis(void);

int main(void)
{

	Stm32_Clock_Init(9);			  /*ϵͳʱ�ӳ�ʼ��*/
	delay_init(72);				  /*��ʱ��ʼ��*/
	LcdInit();					  /*12864��ʼ��*/

	while(1)
	{
		StartDis();
	}
}

void StartDis(void)
{
	LcdDisPic(&Photo1[0]);		  /*��ʾ��ʼ��ӭ����*/
	delay_ms(1000);				  /*��ʱ��ʾ3��*/
	delay_ms(1000);				  //72M�� delay_ms();��������ʽ���� <= 1864 
	delay_ms(1000);				  //����д��3����ʱһ�����ʱ
	LcdDisPic(&Photo2[0]);		  /*��ʾ��ʤ����*/
	delay_ms(1000);				  /*��ʱ��ʾ3��*/
	delay_ms(1000);				  //72M�� delay_ms();��������ʽ���� <= 1864 
	delay_ms(1000);				  //����д��3����ʱһ�����ʱ
}

