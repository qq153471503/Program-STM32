#include "lcd12864.h"
#include "delay.h"
#include "picture.h"

/*��˵����
delay_ms(1000);				  //72M�� delay_ms();��������ʽ���� <= 1864 
12864��8λ���ݿ� ��ȡд���ٶȽ�������������ȴ�һ��ʱ����ܿ���ͼ��*/


int main(void)
{
	SystemInit();
	delay_init();
	LcdInit();
	LcdCleanScreen();				/*����*/
	
	LcdShowPic(&Photo1[0]);		  /*��ʾ��ʼ��ӭ����*/
}

