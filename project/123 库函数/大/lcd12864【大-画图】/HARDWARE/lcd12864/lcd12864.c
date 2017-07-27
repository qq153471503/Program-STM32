#include "lcd12864.h"
#include "delay.h"


void LcdWriteCmd(u8 cmd)
{
	lcdrs0;
	delay_us(10);
	lcdrw0;
	delay_us(10);
	
	LcdData = cmd;
	lcden1;
	delay_ms(2);
	lcden0;
	delay_ms(2);
}
void LcdWriteData(u8 dat)
{
	lcdrs1;
	delay_us(10);
	lcdrw0;
	delay_us(10);
	
	LcdData = dat;
	lcden1;
	delay_ms(2);
	lcden0;
	delay_ms(2);
}
void LcdInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructcre;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD,ENABLE);

	GPIO_InitStructcre.GPIO_Pin = (GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2);
	GPIO_InitStructcre.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructcre.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructcre);
	
	GPIO_InitStructcre.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructcre.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructcre.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&GPIO_InitStructcre);

	lcden0;
	delay_us(10);
	lcdrw0;
	delay_us(10);
	lcdrs0;
	delay_ms(100);
	
	LcdWriteCmd(0x30);
	LcdWriteCmd(0x0c);
	LcdWriteCmd(0x01);
}
void LcdShowPic(u8 *a)	   
{
	u8 i,j;
	
	LcdWriteCmd(0x34);      //������չָ������رջ�ͼ��ʾ��
	for(i=0;i<32;i++)			  //��ΪLCD��������32������д��ʮ����
	{
		LcdWriteCmd(0x80+i);		  //��д��������Y��ֵ
		LcdWriteCmd(0x80);		  //��д�������X��ֵ
		for(j=0; j<16; j++)		  //��������16λ��ÿλд�������ֽڵĵ����ݣ�Ҳ
		{						  //��д��32����Ϊ��д�������ֽ�֮����������
			LcdWriteData(*a); 	      //����1�����ԾͲ����ٴ�д���ַ�ˡ�
			a++;
		}
	}  
	for(i=0; i<32; i++)			  //��ΪLCD��������32������д��ʮ����
	{
		
		LcdWriteCmd(0x80+i);		  //��д��������Y��ֵ
		LcdWriteCmd(0x88);		  //��д�������X��ֵ
		for(j=0; j<16; j++)		  //��������16λ��ÿλд�������ֽڵĵ����ݣ�Ҳ
		{						  //��д��32����Ϊ��д�������ֽ�֮����������
			LcdWriteData(*a); 	      //����1�����ԾͲ����ٴ�д���ַ�ˡ�
			a++;		
		}
	}	
	LcdWriteCmd(0x36);	   //����ʾ
	LcdWriteCmd(0x30);  	   //ת�ػ���ָ�
}
void LcdCleanScreen(void)
{
	u8 i,j;
	LcdWriteCmd(0x34);	  //������չָ�

	for(i=0;i<32;i++)			  //��ΪLCD��������32������д��ʮ����
	{
		LcdWriteCmd(0x80+i);		  //��д��������Y��ֵ
		LcdWriteCmd(0x80);		  //��д�������X��ֵ
		for(j=0; j<32; j++)		  //��������16λ��ÿλд�������ֽڵĵ����ݣ�Ҳ
		{						  //��д��32����Ϊ��д�������ֽ�֮����������
			LcdWriteData(0x00);	  //����1�����ԾͲ����ٴ�д���ַ�ˡ�	   0xff ����Ļ����
		}
	}

	LcdWriteCmd(0x36);	 //0x36��չָ������򿪻�ͼ��ʾ
	delay_ms(10);
	LcdWriteCmd(0x30);	 //�ָ�����ָ�
}
void LcdWriteChar(u8 cmd,u8 dat)
{
	LcdWriteCmd(cmd);
	LcdWriteData(dat);
}




